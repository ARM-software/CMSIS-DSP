/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Python Wrapper
 * Title:        cmsismodule.h
 * Description:  C code for the CMSIS-DSP Python wrapper
 *
 * $Date:        27 April 2021
 * $Revision:    V1.0
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

#define MODNAME "cmsisdsp_distance"
#define MODINITNAME cmsisdsp_distance

#include "cmsisdsp_module.h"
MATRIXFROMNUMPY(f32,float32_t,double,NPY_DOUBLE);
CREATEMATRIX(f32,float32_t);
NUMPYARRAYFROMMATRIX(f32,NPY_FLOAT);

MATRIXFROMNUMPY(q7,q7_t,int8_t,NPY_BYTE);
CREATEMATRIX(q7,q7_t);
NUMPYARRAYFROMMATRIX(q7,NPY_BYTE);


NUMPYVECTORFROMBUFFER(f32,float32_t,NPY_FLOAT);


void typeRegistration(PyObject *module) {

 
}

#define FLOATDIST(NAME)                                                \
static PyObject *                                                      \
cmsis_arm_##NAME##_f32(PyObject *obj, PyObject *args)                  \
{                                                                      \
                                                                       \
  PyObject *pSrcA=NULL;                                                \
  float32_t *pSrcA_converted=NULL;                                     \
  PyObject *pSrcB=NULL;                                                \
  float32_t *pSrcB_converted=NULL;                                     \
  uint32_t blockSize;                                                  \
  float32_t result;                                               \
                                                                       \
  if (PyArg_ParseTuple(args,"OO",&pSrcA,&pSrcB))                       \
  {                                                                    \
                                                                       \
    GETARGUMENT(pSrcA,NPY_DOUBLE,double,float32_t);                    \
    GETARGUMENT(pSrcB,NPY_DOUBLE,double,float32_t);                    \
    blockSize = arraySizepSrcA ;                                       \
                                                                       \
                                                                       \
                                                                       \
    result=arm_##NAME##_f32(pSrcA_converted,pSrcB_converted,blockSize);\
    PyObject* resultOBJ=Py_BuildValue("f",result);                     \
                                                                       \
    PyObject *pythonResult = Py_BuildValue("O",resultOBJ);             \
                                                                       \
    FREEARGUMENT(pSrcA_converted);                                     \
    FREEARGUMENT(pSrcB_converted);                                     \
    Py_DECREF(resultOBJ);                                              \
    return(pythonResult);                                              \
                                                                       \
  }                                                                    \
  return(NULL);                                                        \
}

#define FLOAT64DIST(NAME)                                                \
static PyObject *                                                      \
cmsis_arm_##NAME##_f64(PyObject *obj, PyObject *args)                  \
{                                                                      \
                                                                       \
  PyObject *pSrcA=NULL;                                                \
  float64_t *pSrcA_converted=NULL;                                     \
  PyObject *pSrcB=NULL;                                                \
  float64_t *pSrcB_converted=NULL;                                     \
  uint32_t blockSize;                                                  \
  float64_t result;                                               \
                                                                       \
  if (PyArg_ParseTuple(args,"OO",&pSrcA,&pSrcB))                       \
  {                                                                    \
                                                                       \
    GETARGUMENT(pSrcA,NPY_DOUBLE,double,float64_t);                    \
    GETARGUMENT(pSrcB,NPY_DOUBLE,double,float64_t);                    \
    blockSize = arraySizepSrcA ;                                       \
                                                                       \
                                                                       \
                                                                       \
    result=arm_##NAME##_f64(pSrcA_converted,pSrcB_converted,blockSize);\
    PyObject* resultOBJ=Py_BuildValue("d",result);                     \
                                                                       \
    PyObject *pythonResult = Py_BuildValue("O",resultOBJ);             \
                                                                       \
    FREEARGUMENT(pSrcA_converted);                                     \
    FREEARGUMENT(pSrcB_converted);                                     \
    Py_DECREF(resultOBJ);                                              \
    return(pythonResult);                                              \
                                                                       \
  }                                                                    \
  return(NULL);                                                        \
}

FLOAT64DIST(chebyshev_distance);
FLOAT64DIST(cityblock_distance);
FLOAT64DIST(cosine_distance);
FLOAT64DIST(euclidean_distance);


FLOATDIST(braycurtis_distance);
FLOATDIST(canberra_distance);
FLOATDIST(chebyshev_distance);
FLOATDIST(cityblock_distance);
FLOATDIST(correlation_distance);
FLOATDIST(cosine_distance);
FLOATDIST(euclidean_distance);
FLOATDIST(jensenshannon_distance);

static PyObject *                                                    
cmsis_arm_minkowski_distance_f32(PyObject *obj, PyObject *args)                  
{                                                                      
                                                                       
  PyObject *pSrcA=NULL;                                                
  float32_t *pSrcA_converted=NULL;                                     
  PyObject *pSrcB=NULL;                                                
  float32_t *pSrcB_converted=NULL;                                     
  int32_t w;                                                
  uint32_t blockSize;                                                  
  float32_t result;                                               
                                                                       
  if (PyArg_ParseTuple(args,"OOi",&pSrcA,&pSrcB,&w))                       
  {                                                                    
                                                                       
    GETARGUMENT(pSrcA,NPY_DOUBLE,double,float32_t);                    
    GETARGUMENT(pSrcB,NPY_DOUBLE,double,float32_t);                    

    blockSize = arraySizepSrcA ;                                       
                                                                       
                                                                       
                                                                       
    result=arm_minkowski_distance_f32(pSrcA_converted,pSrcB_converted,w,blockSize);
    PyObject* resultOBJ=Py_BuildValue("f",result);                     
                                                                       
    PyObject *pythonResult = Py_BuildValue("O",resultOBJ);             
                                                                       
    FREEARGUMENT(pSrcA_converted);                                     
    FREEARGUMENT(pSrcB_converted);                                     

    Py_DECREF(resultOBJ);                                              
    return(pythonResult);                                              
                                                                       
  }                                                                    
  return(NULL);                                                        
}


#define INTDIST(NAME)                                               \
static PyObject *                                                   \
cmsis_arm_##NAME (PyObject *obj, PyObject *args)                   \
{                                                                   \
                                                                    \
  PyObject *pSrcA=NULL;                                             \
  uint32_t *pSrcA_converted=NULL;                                   \
  PyObject *pSrcB=NULL;                                             \
  uint32_t *pSrcB_converted=NULL;                                   \
  uint32_t blockSize;                                               \
  float32_t result;                                            \
                                                                    \
                                                                    \
  if (PyArg_ParseTuple(args,"OOi",&pSrcA,&pSrcB,&blockSize))        \
  {                                                                 \
                                                                    \
    GETARGUMENT(pSrcA,NPY_UINT32,uint32_t,uint32_t);                \
    GETARGUMENT(pSrcB,NPY_UINT32,uint32_t,uint32_t);                \
                                                                    \
                                                                    \
                                                                    \
    result=arm_##NAME (pSrcA_converted,pSrcB_converted,blockSize); \
    PyObject* resultOBJ=Py_BuildValue("f",result);                  \
                                                                    \
    PyObject *pythonResult = Py_BuildValue("O",resultOBJ);          \
                                                                    \
    FREEARGUMENT(pSrcA_converted);                                  \
    FREEARGUMENT(pSrcB_converted);                                  \
    Py_DECREF(resultOBJ);                                           \
    return(pythonResult);                                           \
                                                                    \
  }                                                                 \
  return(NULL);                                                     \
}


INTDIST(dice_distance);
INTDIST(hamming_distance);
INTDIST(jaccard_distance);
INTDIST(kulsinski_distance);
INTDIST(rogerstanimoto_distance);
INTDIST(russellrao_distance);
INTDIST(sokalmichener_distance);
INTDIST(sokalsneath_distance);
INTDIST(yule_distance);

static PyObject *
cmsis_arm_dtw_init_window_q7(PyObject *obj, 
                             PyObject *args)
{

  PyObject *pSrc=NULL; // input
  int32_t winType;
  int32_t winSize;
  arm_matrix_instance_q7 pSrc_converted; // input
   

  if (PyArg_ParseTuple(args,"iiO",&winType,&winSize,&pSrc))
  {

    q7MatrixFromNumpy(&pSrc_converted,pSrc);
    uint32_t row = pSrc_converted.numCols ;
    uint32_t column = pSrc_converted.numRows ;

    arm_status returnValue = 
    arm_dtw_init_window_q7(winType,
                           winSize,
                           &pSrc_converted
                           );
    PyObject* theReturnOBJ=Py_BuildValue("i",returnValue);
    PyObject* dstOBJ=NumpyArrayFromq7Matrix(&pSrc_converted);

    PyObject *pythonResult = Py_BuildValue("OO",theReturnOBJ,dstOBJ);

    Py_DECREF(theReturnOBJ);
    Py_DECREF(dstOBJ);
    return(pythonResult);

  }
  Py_RETURN_NONE;
}

static PyObject *
cmsis_arm_dtw_distance_f32(PyObject *obj, 
                    PyObject *args)
{

  PyObject *pDist=NULL; // input
  arm_matrix_instance_f32 pDist_converted; // input
   
  PyObject *pWin=NULL; // input
  arm_matrix_instance_q7 pWin_converted; // input
  arm_matrix_instance_q7 *pWinMatrix;

  arm_matrix_instance_f32 dtw_converted;


  if (PyArg_ParseTuple(args,"OO",&pDist,&pWin))
  {

    f32MatrixFromNumpy(&pDist_converted,pDist);
    if (pWin != Py_None)
    {
       q7MatrixFromNumpy(&pWin_converted,pWin);
       pWinMatrix = &pWin_converted;
    }
    else
    {
        pWinMatrix = NULL;
    }

    uint32_t column = pDist_converted.numCols ;
    uint32_t row = pDist_converted.numRows ;
    createf32Matrix(&dtw_converted,row,column);
    float32_t distance;

    arm_status returnValue = 
    arm_dtw_distance_f32(&pDist_converted,
                         pWinMatrix,
                         &dtw_converted,
                         &distance
                         );

    
    PyObject* theReturnOBJ=Py_BuildValue("i",returnValue);
    PyObject* distOBJ=Py_BuildValue("f",distance);

    PyObject* dstOBJ=NumpyArrayFromf32Matrix(&dtw_converted);


    PyObject *pythonResult = Py_BuildValue("OOO",theReturnOBJ,distOBJ,dstOBJ);

    Py_DECREF(theReturnOBJ);
    Py_DECREF(distOBJ);

    FREEMATRIX(&pDist_converted);
    if (pWinMatrix)
    {
        FREEMATRIX(pWinMatrix);
    }
    Py_DECREF(dstOBJ);

    return(pythonResult);

  }
  Py_RETURN_NONE;
}

static PyObject *
cmsis_arm_dtw_path_f32(PyObject *obj, 
                       PyObject *args)
{
  PyObject *pCost=NULL; // input
  arm_matrix_instance_f32 pCost_converted; // input
  int16_t *pDst=NULL; // output

  if (PyArg_ParseTuple(args,"O",&pCost))
  {
     f32MatrixFromNumpy(&pCost_converted,pCost);

     uint32_t pathLength;
     int32_t blockSize;
     blockSize=2*(pCost_converted.numRows+pCost_converted.numCols);
     pDst=PyMem_Malloc(sizeof(int16_t)*blockSize);


     arm_dtw_path_f32(&pCost_converted,
                      pDst,
                      &pathLength);

     INT16ARRAY1(pDstOBJ,2*pathLength,pDst);

     PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

     FREEMATRIX(&pCost_converted);
     Py_DECREF(pDstOBJ);
     return(pythonResult);
  }
  Py_RETURN_NONE;
}

static PyMethodDef CMSISDSPMethods[] = {

    {"arm_braycurtis_distance_f32",  cmsis_arm_braycurtis_distance_f32, METH_VARARGS,""},
    {"arm_canberra_distance_f32"  ,  cmsis_arm_canberra_distance_f32, METH_VARARGS,""},
    {"arm_chebyshev_distance_f32" ,  cmsis_arm_chebyshev_distance_f32, METH_VARARGS,""},
    {"arm_chebyshev_distance_f64" ,  cmsis_arm_chebyshev_distance_f64, METH_VARARGS,""},

    {"arm_cityblock_distance_f32",  cmsis_arm_cityblock_distance_f32, METH_VARARGS,""},
    {"arm_cityblock_distance_f64",  cmsis_arm_cityblock_distance_f64, METH_VARARGS,""},

    {"arm_correlation_distance_f32",  cmsis_arm_correlation_distance_f32, METH_VARARGS,""},

    {"arm_cosine_distance_f32",  cmsis_arm_cosine_distance_f32, METH_VARARGS,""},
    {"arm_cosine_distance_f64",  cmsis_arm_cosine_distance_f64, METH_VARARGS,""},

    {"arm_euclidean_distance_f32",  cmsis_arm_euclidean_distance_f32, METH_VARARGS,""},
    {"arm_euclidean_distance_f64",  cmsis_arm_euclidean_distance_f64, METH_VARARGS,""},

    {"arm_jensenshannon_distance_f32",  cmsis_arm_jensenshannon_distance_f32, METH_VARARGS,""},
    {"arm_minkowski_distance_f32",  cmsis_arm_minkowski_distance_f32, METH_VARARGS,""},

    {"arm_dice_distance",cmsis_arm_dice_distance, METH_VARARGS,""},
    {"arm_hamming_distance",cmsis_arm_hamming_distance, METH_VARARGS,""},
    {"arm_jaccard_distance",cmsis_arm_jaccard_distance, METH_VARARGS,""},
    {"arm_kulsinski_distance",cmsis_arm_kulsinski_distance, METH_VARARGS,""},
    {"arm_rogerstanimoto_distance",cmsis_arm_rogerstanimoto_distance, METH_VARARGS,""},
    {"arm_russellrao_distance",cmsis_arm_russellrao_distance, METH_VARARGS,""},
    {"arm_sokalmichener_distance",cmsis_arm_sokalmichener_distance, METH_VARARGS,""},
    {"arm_sokalsneath_distance",cmsis_arm_sokalsneath_distance, METH_VARARGS,""},
    {"arm_yule_distance",cmsis_arm_yule_distance, METH_VARARGS,""},

    {"arm_dtw_init_window_q7",  cmsis_arm_dtw_init_window_q7, METH_VARARGS,""},
    {"arm_dtw_distance_f32",  cmsis_arm_dtw_distance_f32, METH_VARARGS,""},
    {"arm_dtw_path_f32",  cmsis_arm_dtw_path_f32, METH_VARARGS,""},

    {"error_out", (PyCFunction)error_out, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

#ifdef IS_PY3K
static int cmsisdsp_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int cmsisdsp_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        MODNAME,
        NULL,
        sizeof(struct module_state),
        CMSISDSPMethods,
        NULL,
        cmsisdsp_traverse,
        cmsisdsp_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
CAT(PyInit_,MODINITNAME)(void)


#else
#define INITERROR return

void CAT(init,MODINITNAME)(void)
#endif
{
    import_array();

  #ifdef IS_PY3K
    PyObject *module = PyModule_Create(&moduledef);
  #else
    PyObject *module = Py_InitModule(MODNAME, CMSISDSPMethods);
  #endif

  if (module == NULL)
      INITERROR;
  struct module_state *st = GETSTATE(module);
  
  st->error = PyErr_NewException(MODNAME".Error", NULL, NULL);
  if (st->error == NULL) {
      Py_DECREF(module);
      INITERROR;
  }


  typeRegistration(module);

  #ifdef IS_PY3K
    return module;
  #endif
}
