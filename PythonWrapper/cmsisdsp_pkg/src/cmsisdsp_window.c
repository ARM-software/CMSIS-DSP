/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Python Wrapper
 * Title:        cmsismodule.h
 * Description:  C code for the CMSIS-DSP Python wrapper
 *
 * $Date:        15 December 2022
 * $Revision:    v1.15.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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

#define MODNAME "cmsisdsp_window"
#define MODINITNAME cmsisdsp_window

#include "cmsisdsp_module.h"


NUMPYVECTORFROMBUFFER(f32,float32_t,NPY_FLOAT);


void typeRegistration(PyObject *module) {

 
}


static PyObject *
cmsis_arm_welch_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_welch_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_welch_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_welch_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_bartlett_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_bartlett_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_bartlett_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_bartlett_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hamming_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hamming_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hamming_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hamming_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hanning_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hanning_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hanning_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hanning_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall3_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall3_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall3_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall3_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall4_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall4_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall4_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall4_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall3a_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall3a_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall3a_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall3a_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall3b_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall3b_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall3b_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall3b_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall4a_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall4a_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall4a_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall4a_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_blackman_harris_92db_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_blackman_harris_92db_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_blackman_harris_92db_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_blackman_harris_92db_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall4b_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall4b_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall4b_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall4b_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_nuttall4c_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_nuttall4c_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_nuttall4c_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_nuttall4c_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft90d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft90d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft90d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft90d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft95_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft95_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft95_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft95_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft116d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft116d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft116d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft116d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft144d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft144d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft144d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft144d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft169d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft169d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft169d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft169d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft196d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft196d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft196d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft196d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft223d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft223d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft223d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft223d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyObject *
cmsis_arm_hft248d_f32(PyObject *obj, PyObject *args)
{

  
  float32_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float32_t)*nb);


    arm_hft248d_f32(pDst,nb);
    FLOATARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}

static PyObject *
cmsis_arm_hft248d_f64(PyObject *obj, PyObject *args)
{

  
  float64_t *pDst=NULL; // output
  int nb; // input

  if (PyArg_ParseTuple(args,"i",&nb))
  {

    pDst=PyMem_Malloc(sizeof(float64_t)*nb);


    arm_hft248d_f64(pDst,nb);
    FLOAT64ARRAY1(pDstOBJ,nb,pDst);

    PyObject *pythonResult = Py_BuildValue("O",pDstOBJ);

    Py_DECREF(pDstOBJ);
    return(pythonResult);

  }
  return(NULL);
}


static PyMethodDef CMSISDSPMethods[] = {






{"arm_welch_f32" ,  cmsis_arm_welch_f32, METH_VARARGS,""},
{"arm_welch_f64" ,  cmsis_arm_welch_f64, METH_VARARGS,""},


{"arm_bartlett_f32" ,  cmsis_arm_bartlett_f32, METH_VARARGS,""},
{"arm_bartlett_f64" ,  cmsis_arm_bartlett_f64, METH_VARARGS,""},


{"arm_hamming_f32" ,  cmsis_arm_hamming_f32, METH_VARARGS,""},
{"arm_hamming_f64" ,  cmsis_arm_hamming_f64, METH_VARARGS,""},


{"arm_hanning_f32" ,  cmsis_arm_hanning_f32, METH_VARARGS,""},
{"arm_hanning_f64" ,  cmsis_arm_hanning_f64, METH_VARARGS,""},


{"arm_nuttall3_f32" ,  cmsis_arm_nuttall3_f32, METH_VARARGS,""},
{"arm_nuttall3_f64" ,  cmsis_arm_nuttall3_f64, METH_VARARGS,""},


{"arm_nuttall4_f32" ,  cmsis_arm_nuttall4_f32, METH_VARARGS,""},
{"arm_nuttall4_f64" ,  cmsis_arm_nuttall4_f64, METH_VARARGS,""},


{"arm_nuttall3a_f32" ,  cmsis_arm_nuttall3a_f32, METH_VARARGS,""},
{"arm_nuttall3a_f64" ,  cmsis_arm_nuttall3a_f64, METH_VARARGS,""},


{"arm_nuttall3b_f32" ,  cmsis_arm_nuttall3b_f32, METH_VARARGS,""},
{"arm_nuttall3b_f64" ,  cmsis_arm_nuttall3b_f64, METH_VARARGS,""},


{"arm_nuttall4a_f32" ,  cmsis_arm_nuttall4a_f32, METH_VARARGS,""},
{"arm_nuttall4a_f64" ,  cmsis_arm_nuttall4a_f64, METH_VARARGS,""},


{"arm_blackman_harris_92db_f32" ,  cmsis_arm_blackman_harris_92db_f32, METH_VARARGS,""},
{"arm_blackman_harris_92db_f64" ,  cmsis_arm_blackman_harris_92db_f64, METH_VARARGS,""},


{"arm_nuttall4b_f32" ,  cmsis_arm_nuttall4b_f32, METH_VARARGS,""},
{"arm_nuttall4b_f64" ,  cmsis_arm_nuttall4b_f64, METH_VARARGS,""},


{"arm_nuttall4c_f32" ,  cmsis_arm_nuttall4c_f32, METH_VARARGS,""},
{"arm_nuttall4c_f64" ,  cmsis_arm_nuttall4c_f64, METH_VARARGS,""},


{"arm_hft90d_f32" ,  cmsis_arm_hft90d_f32, METH_VARARGS,""},
{"arm_hft90d_f64" ,  cmsis_arm_hft90d_f64, METH_VARARGS,""},


{"arm_hft95_f32" ,  cmsis_arm_hft95_f32, METH_VARARGS,""},
{"arm_hft95_f64" ,  cmsis_arm_hft95_f64, METH_VARARGS,""},


{"arm_hft116d_f32" ,  cmsis_arm_hft116d_f32, METH_VARARGS,""},
{"arm_hft116d_f64" ,  cmsis_arm_hft116d_f64, METH_VARARGS,""},


{"arm_hft144d_f32" ,  cmsis_arm_hft144d_f32, METH_VARARGS,""},
{"arm_hft144d_f64" ,  cmsis_arm_hft144d_f64, METH_VARARGS,""},


{"arm_hft169d_f32" ,  cmsis_arm_hft169d_f32, METH_VARARGS,""},
{"arm_hft169d_f64" ,  cmsis_arm_hft169d_f64, METH_VARARGS,""},


{"arm_hft196d_f32" ,  cmsis_arm_hft196d_f32, METH_VARARGS,""},
{"arm_hft196d_f64" ,  cmsis_arm_hft196d_f64, METH_VARARGS,""},


{"arm_hft223d_f32" ,  cmsis_arm_hft223d_f32, METH_VARARGS,""},
{"arm_hft223d_f64" ,  cmsis_arm_hft223d_f64, METH_VARARGS,""},


{"arm_hft248d_f32" ,  cmsis_arm_hft248d_f32, METH_VARARGS,""},
{"arm_hft248d_f64" ,  cmsis_arm_hft248d_f64, METH_VARARGS,""},

   
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
