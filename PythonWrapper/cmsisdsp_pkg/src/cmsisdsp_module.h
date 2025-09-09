/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Python Wrapper
 * Title:        cmsismodule.c
 * Description:  C code for the CMSIS-DSP Python wrapper
 *
 * $Date:        27 April 2021
 * $Revision:    VV1.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CMSISMODULE_H
#define CMSISMODULE_H
#define NPY_NO_DEPRECATED_API NPY_1_23_API_VERSION

#include <numpy/numpyconfig.h>

// Check it is built with right version
// (should be backward compatible down to 1.23.5)
// https://github.com/numpy/numpy/blob/main/numpy/_core/include/numpy/numpyconfig.h
#if (NPY_API_VERSION != NPY_2_3_API_VERSION    )
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#pragma message ("Building with numpy version " STR(NPY_API_VERSION))
#error "Error building with wrong NumPy API version"
#endif

#ifdef WIN
#pragma warning( disable : 4013 ) 
#pragma warning( disable : 4244 ) 
#endif

#include <Python.h>

#define CAT1(A,B) A##B
#define CAT(A,B) CAT1(A,B)


#include "arm_math.h"


#include <numpy/arrayobject.h>
#include <numpy/ndarraytypes.h>

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *
error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

#define DSPType(name,thenewfunc,deallocfunc,initfunc,methods)\
static PyTypeObject dsp_##name##Type = {          \
    PyVarObject_HEAD_INIT(NULL, 0)               \
    .tp_name=MODNAME"." #name,                   \
    .tp_basicsize = sizeof(dsp_##name##Object),   \
    .tp_itemsize = 0,                            \
    .tp_dealloc = (destructor)deallocfunc,       \
    .tp_flags =  Py_TPFLAGS_DEFAULT,           \
    .tp_doc = #name,                             \
    .tp_init = (initproc)initfunc,               \
    .tp_new = (newfunc)thenewfunc,                \
    .tp_methods = methods  \
   };


#define MEMCPY(DST,SRC,NB,FORMAT) \
for(memCpyIndex = 0; memCpyIndex < (NB) ; memCpyIndex++)\
{                                \
  (DST)[memCpyIndex] = (FORMAT)(SRC)[memCpyIndex];       \
}

#define GETFIELD(NAME,FIELD,FORMAT)                                           \
static PyObject *                                                             \
Method_##NAME##_##FIELD(dsp_##NAME##Object *self, PyObject *ignored)\
{                                                                             \
    return(Py_BuildValue(FORMAT,self->instance->FIELD));                      \
}                                                                             
    
#define GETFIELDARRAY(NAME,FIELD,FORMAT)                                           \
static PyObject *                                                             \
Method_##NAME##_##FIELD(dsp_##NAME##Object *self, PyObject *ignored)\
{                                                                             \
    return(specific_##NAME##_##FIELD(self->instance));                      \
}  

#define INITARRAYFIELD(FIELD,FORMAT,SRCFORMAT,DSTFORMAT)                         \
    if (FIELD)                                                                \
    {                                                                         \
       PyArray_Descr *desct=PyArray_DescrFromType(FORMAT);                    \
       PyArrayObject *FIELD##c = (PyArrayObject *)PyArray_FromAny(FIELD,desct,\
        1,0,NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_FORCECAST, \
        NULL);                                                                \
       if (FIELD##c)                                                          \
       {                                                                      \
           uint32_t memCpyIndex; \
           SRCFORMAT *f=(SRCFORMAT*)PyArray_DATA(FIELD##c);                   \
           uint32_t n = PyArray_SIZE(FIELD##c);                               \
           self->instance->FIELD =PyMem_Malloc(sizeof(DSTFORMAT)*n);                \
           MEMCPY((DSTFORMAT*)self->instance->FIELD ,f,n,DSTFORMAT);                      \
           Py_DECREF(FIELD##c);                                               \
       }                                                                      \
    }
#define GETCARRAY(PYVAR,CVAR,FORMAT,SRCFORMAT,DSTFORMAT)                                \
    if (PYVAR)                                                                \
    {                                                                         \
       PyArray_Descr *desct=PyArray_DescrFromType(FORMAT);                    \
       PyArrayObject *PYVAR##c = (PyArrayObject *)PyArray_FromAny(PYVAR,desct,\
        1,0,NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_FORCECAST, \
        NULL);                                                                \
       if (PYVAR##c)                                                          \
       {                                                                      \
           uint32_t memCpyIndex; \
           SRCFORMAT *f=(SRCFORMAT*)PyArray_DATA(PYVAR##c);                         \
           uint32_t n = PyArray_SIZE(PYVAR##c);                               \
           CVAR =PyMem_Malloc(sizeof(DSTFORMAT)*n);                                 \
           MEMCPY(CVAR ,f,n,DSTFORMAT);                               \
           Py_DECREF(PYVAR##c);                                               \
       }                                                                      \
    }

// Get array argument using a copy that is also doing data conversion
// In numpy, we don't want to force the use of float when the default is double
// to make the use of the API easier.
// So double arrays will be converted to float32 array by this macro
// and the wrapper code is using NPY_DOUBLE for float32 CMSIS-DSP functions
// when using this macro
#define GETARGUMENT(FIELD,FORMAT,SRCFORMAT,DSTFORMAT)                          \
    uint32_t arraySize##FIELD=0;                                               \
    if (FIELD)                                                                 \
    {                                                                          \
       PyArray_Descr *desct=PyArray_DescrFromType(FORMAT);                     \
       PyArrayObject *FIELD##c = (PyArrayObject *)PyArray_FromAny(FIELD,desct, \
        1,0,NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_FORCECAST,  \
        NULL);                                                                 \
       if (FIELD##c)                                                           \
       {                                                                       \
           uint32_t memCpyIndex; \
           SRCFORMAT *f=(SRCFORMAT*)PyArray_DATA(FIELD##c);                    \
           arraySize##FIELD = PyArray_SIZE(FIELD##c);                          \
           FIELD##_converted =PyMem_Malloc(sizeof(DSTFORMAT)*arraySize##FIELD);\
           MEMCPY(FIELD##_converted ,f,arraySize##FIELD,DSTFORMAT);            \
           Py_DECREF(FIELD##c);                                                \
       }                                                                       \
    }

#define ACCESSARRAY(DST,FIELD,FORMAT,SRCFORMAT)                               \
   {                                                                          \
       PyArray_Descr *desct=PyArray_DescrFromType(FORMAT);                    \
       FIELD##c = PyArray_FromAny(FIELD,desct,                                \
        1,0,NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_FORCECAST, \
        NULL);                                                                \
       if (FIELD##c != NULL)                                                  \
       {                                                                      \
           DST=(SRCFORMAT*)PyArray_DATA((PyArrayObject*)FIELD##c);            \
       }                                                                      \
    }
       
#define ARRAYNOMOREUSED(FIELD)  \
       if (FIELD##c)            \
       {                        \
        Py_DECREF((PyObject*)FIELD##c); \
       }

/*

For management of temporary buffers in Neon version.
If the temporary buffer is passed as named argument, it is used. 
Otherwise, a temporary buffer is allocated in the function to 
minimize the change required when upgrading a Python script to the 
Neon version of the extension.

*/

#define ALLOC_OR_GET_TMP(TMP,PYTMP,NBSAMPLES,NUMPY_DT,CMSIS_DT) \
       CMSIS_DT *TMP=NULL;                                              \
       PyObject *PYTMP##c=NULL;                                 \
                                                                \
       if (PYTMP)                                               \
       {                                                        \
         ACCESSARRAY(TMP,PYTMP,NUMPY_DT,CMSIS_DT);              \
       }                                                        \
       else                                                     \
       {                                                        \
         TMP = PyMem_Malloc(NBSAMPLES*sizeof(CMSIS_DT));        \
       }
   
#define FREE_OR_RELEASE(TMP,PYTMP) \
       if (PYTMP)                  \
       {                           \
           ARRAYNOMOREUSED(PYTMP); \
       }                           \
       else                        \
       {                           \
          PyMem_Free(TMP);         \
       }

#define FREEARGUMENT(FIELD) \
    PyMem_Free(FIELD)

#ifdef IS_PY3K
#define ADDTYPE(name)                                               \
    if (PyType_Ready(&dsp_##name##Type) < 0)                         \
        return;                                              \
                                                                    \
    Py_INCREF(&dsp_##name##Type);                                    \
    PyModule_AddObject(module, #name, (PyObject *)&dsp_##name##Type);
#else
#define ADDTYPE(name)                                               \
    if (PyType_Ready(&dsp_##name##Type) < 0)                         \
        return;                                                     \
                                                                    \
    Py_INCREF(&dsp_##name##Type);                                    \
    PyModule_AddObject(module, #name, (PyObject *)&dsp_##name##Type);
#endif

void capsule_cleanup(PyObject *capsule) {
    void *memory = PyCapsule_GetPointer(capsule, "cmsisdsp capsule");
    PyMem_Free(memory);
}

#define FLOATARRAY2(OBJ,NB1,NB2,DATA)                                                       \
    npy_intp dims##OBJ[2];                                                                       \
    dims##OBJ[0]=NB1;                                                                            \
    dims##OBJ[1]=NB2;                                                                            \
    const int ND##OBJ=2;                                                                         \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_FLOAT, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define FLOATARRAY1(OBJ,NB1,DATA)                                                                     \
    npy_intp dims##OBJ[1];                                                                            \
    dims##OBJ[0]=NB1;                                                                                 \
    const int ND##OBJ=1;                                                                              \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_FLOAT, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define FLOAT64ARRAY1(OBJ,NB1,DATA)                                                          \
    npy_intp dims##OBJ[1];                                                                        \
    dims##OBJ[0]=NB1;                                                                             \
    const int ND##OBJ=1;                                                                          \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_DOUBLE, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define UINT32ARRAY1(OBJ,NB1,DATA)                                                           \
    npy_intp dims##OBJ[1];                                                                        \
    dims##OBJ[0]=NB1;                                                                             \
    const int ND##OBJ=1;                                                                          \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_UINT32, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define INT32ARRAY1(OBJ,NB1,DATA)                                                           \
    npy_intp dims##OBJ[1];                                                                       \
    dims##OBJ[0]=NB1;                                                                            \
    const int ND##OBJ=1;                                                                         \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_INT32, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define INT16ARRAY1(OBJ,NB1,DATA)                                                           \
    npy_intp dims##OBJ[1];                                                                       \
    dims##OBJ[0]=NB1;                                                                            \
    const int ND##OBJ=1;                                                                         \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_INT16, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define INT8ARRAY1(OBJ,NB1,DATA)                                                           \
    npy_intp dims##OBJ[1];                                                                      \
    dims##OBJ[0]=NB1;                                                                           \
    const int ND##OBJ=1;                                                                        \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPY_BYTE, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define TYP_ARRAY1(OBJ,NB1,DATA,NPYTYPE)                                                  \
    npy_intp dims##OBJ[1];                                                                     \
    dims##OBJ[0]=NB1;                                                                          \
    const int ND##OBJ=1;                                                                       \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND##OBJ, dims##OBJ, NPYTYPE, DATA);\
    PyObject *capsule##OBJ = PyCapsule_New(DATA, "cmsisdsp capsule",capsule_cleanup);                       \
    PyArray_SetBaseObject(OBJ, capsule##OBJ);

#define MATRIXFROMNUMPY(EXT,TYP,SRCTYPE,NUMPYTYPE)                                   \
void EXT##MatrixFromNumpy(arm_matrix_instance_##EXT *s,PyObject *o)                   \
{                                                                            \
    s->pData=NULL;                                                           \
    s->numRows=0;                                                            \
    s->numCols=0;                                                            \
                                                                             \
    PyArray_Descr *desct=PyArray_DescrFromType(NUMPYTYPE);                    \
    PyArrayObject *cdata = (PyArrayObject *)PyArray_FromAny(o,desct,         \
        1,0,NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_FORCECAST, \
        NULL);                                                                \
    if (cdata)                                                               \
    {                                                                        \
           uint32_t memCpyIndex;                                             \
           SRCTYPE *f=(SRCTYPE*)PyArray_DATA(cdata);                           \
           s->numRows=PyArray_DIM(cdata,0);                                  \
           s->numCols=PyArray_DIM(cdata,1);                                  \
           uint32_t nb = PyArray_SIZE(cdata);                                \
           s->pData = PyMem_Malloc(sizeof(TYP)*nb);                                \
           MEMCPY(s->pData ,f,nb,TYP);                                       \
           Py_DECREF(cdata);                                                 \
    }                                                                        \
                                                                             \
                                                                             \
                                                                             \
}



#define CREATEMATRIX(EXT,TYP)                                        \
void create##EXT##Matrix(arm_matrix_instance_##EXT *s,uint32_t r,uint32_t c)\
{                                                                    \
                                                                     \
    s->pData=PyMem_Malloc(sizeof(TYP)*r*c);                                \
    s->numRows=r;                                                    \
    s->numCols=c;                                                    \
}

#define FREEMATRIX(s) PyMem_Free((s)->pData)

#define NUMPYVECTORFROMBUFFER(EXT,CTYPE,NUMPYTYPE_FROMC)                                          \
PyObject *NumpyVectorFrom##EXT##Buffer(CTYPE *ptr,int nb)                                         \
{                                                                                                 \
    npy_intp dims[1];                                                                             \
    const int ND=1;                                                                               \
    dims[0]=nb;                                                                                   \
                                                                                                  \
    void *pDst=PyMem_Malloc(sizeof(CTYPE) *nb);                                                   \
    memcpy((void*)pDst,(void*)ptr,sizeof(CTYPE)*nb);                                              \
                                                                                                  \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND, dims, NUMPYTYPE_FROMC, pDst);\
    PyObject *capsule = PyCapsule_New(pDst, "cmsisdsp capsule",capsule_cleanup);                             \
    PyArray_SetBaseObject(OBJ, capsule);                                                          \
    PyObject *pythonResult = Py_BuildValue("O",OBJ);                                              \
    Py_DECREF(OBJ);                                                                               \
    return(pythonResult);                                                                         \
}




#define NUMPYARRAYFROMMATRIX(EXT,NUMPYTYPE_FROMC)                                                       \
PyObject *NumpyArrayFrom##EXT##Matrix(arm_matrix_instance_##EXT *mat)                                   \
{                                                                                                       \
    npy_intp dims[2];                                                                                   \
    dims[0]=mat->numRows;                                                                               \
    dims[1]=mat->numCols;                                                                               \
    const int ND=2;                                                                                     \
    PyArrayObject *OBJ=(PyArrayObject*)PyArray_SimpleNewFromData(ND, dims, NUMPYTYPE_FROMC, mat->pData);\
    PyObject *capsule = PyCapsule_New(mat->pData, "cmsisdsp capsule",capsule_cleanup);                  \
    PyArray_SetBaseObject(OBJ, capsule);                                                                \
    PyObject *pythonResult = Py_BuildValue("O",OBJ);                                                    \
    Py_DECREF(OBJ);                                                                                     \
    return(pythonResult);                                                                               \
}






#endif /* #ifndef CMSISMODULE_H */
