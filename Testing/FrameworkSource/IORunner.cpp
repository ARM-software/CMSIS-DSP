/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        IORunner.cpp
 * Description:  IORunner
 *
 *               Runner implementation for runner running on device 
 *               under test
 *
 * $Date:        20. June 2019
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
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
#include "Test.h"

#include <string>
#include <cstddef>
#include "IORunner.h"
#include "Error.h"
#include "Timing.h"
#include "arm_math.h"

namespace Client
{
  
      IORunner::IORunner(IO *io,PatternMgr *mgr,  Testing::RunningMode runningMode):m_io(io), m_mgr(mgr)
      {
        this->m_runningMode = runningMode;
        // Set running mode on PatternMgr.
        if (runningMode == Testing::kDumpOnly)
        {
          mgr->setDumpMode();
        }
        if (runningMode == Testing::kTestAndDump)
        {
          mgr->setTestAndDumpMode();
        }

        initCycleMeasurement();
      }

      // Testing.
      // When false we are in dump mode and the failed assertion are ignored
      // (But exception is taken so assert should be at end of the test and not in the
      // middle )
      IORunner::IORunner(IO *io,PatternMgr *mgr):m_io(io), m_mgr(mgr)
      {
        this->m_runningMode = Testing::kTestOnly;
      }

      IORunner::~IORunner()
      {
        
      }

     
      /** Read driver data to control execution of a suite
      */
      Testing::TestStatus IORunner::run(Suite *s) 
      {
        Testing::TestStatus finalResult = Testing::kTestPassed;
        int nbTests = s->getNbTests();
        int failedTests=0;
        Testing::errorID_t error=0;
        unsigned long line = 0;
        Testing::cycles_t cycles=0;
        Testing::nbParameters_t nbParams;

        // Read node identification (suite)
        m_io->ReadIdentification();
        // Read suite nb of parameters 
        nbParams = m_io->ReadNbParameters();

        // Read list of patterns
        m_io->ReadPatternList();
        // Read list of output
        m_io->ReadOutputList();
        // Read list of parameters
        m_io->ReadParameterList(nbParams);

        // Iterate on tests
        for(int i=1; i <= nbTests; i++)
        {
            test t = s->getTest(i);
            Testing::TestStatus result = Testing::kTestPassed;
            error = UNKNOWN_ERROR;
            line = 0;
            cycles = 0;
            Testing::param_t *paramData=NULL;
            Testing::nbParameterEntries_t entries=0;
            std::vector<Testing::param_t> params(nbParams);
            bool canExecute=true;
            int  dataIndex=0;

            // Read test identification (test ID)
            m_io->ReadTestIdentification();
            
            
            if (m_io->hasParam())
            {
               Testing::PatternID_t paramID=m_io->getParamID();
               paramData = m_io->ImportParams(paramID,entries);
               dataIndex = 0;
            }


            while(canExecute)
            {
              canExecute = false; 
              
              if (m_io->hasParam() && paramData)
              {
                // Load new params
                for(int j=0; j < nbParams ; j++)
                {
                  params[j] = paramData[nbParams*dataIndex+j];
                }
                // Update condition for new execution
                dataIndex += 1;
                canExecute = dataIndex < entries;
              }
              // Execute test
              try {     
                // Prepare memory for test
                // setUp will generally load patterns
                // and do specific initialization for the tests
                s->setUp(m_io->CurrentTestID(),params,m_mgr);
                // Run the test
                cycleMeasurementStart();
                (s->*t)();
                cycles=getCycles();
                cycleMeasurementStop();
              } 
              catch(Error &ex)
              {
                 // In dump only mode we ignore the tests 
                 // since the reference patterns are not loaded
                 // so tests will fail.
                 if (this->m_runningMode != Testing::kDumpOnly)
                 {
                    error = ex.errorID;
                    line = ex.lineNumber;
                    result=Testing::kTestFailed;
                 }
              }
              catch (...) { 
                // In dump only mode we ignore the tests 
                // since the reference patterns are not loaded
                // so tests will fail.
                if (this->m_runningMode != Testing::kDumpOnly)
                {
                  result = Testing::kTestFailed;
                  error = UNKNOWN_ERROR;
                  line = 0;
                }
              }
              try { 
                 // Clean memory after this test
                 // May dump output and do specific cleaning for a test
                 s->tearDown(m_io->CurrentTestID(),m_mgr);
              }
              catch(...)
              {
              
              }
              
              // Free all memory of memory manager so that next test
              // is starting in a clean and controlled tests
              m_mgr->freeAll();
  
              // Dump test status to output
              m_io->DispStatus(result,error,line,cycles);
              m_io->DumpParams(params);
            }
            if (paramData)
            {
                free(paramData);
                paramData = NULL;
            }

            if (result == Testing::kTestFailed)
            {
              failedTests ++;
              finalResult = Testing::kTestFailed;
            }
        }
        // Signal end of group processing to output
        m_io->EndGroup();
        return(finalResult);
     }

      /** Read driver data to control execution of a group
      */
      Testing::TestStatus IORunner::run(Group *g) 
      {
        int nbTests = g->getNbContainer();
        int failedTests=0;


        // Read Node identification
        m_io->ReadIdentification();
        

        Testing::TestStatus finalResult = Testing::kTestPassed;
        // Iterate on group elements
        for(int i=1; i <= nbTests; i++)
        {
            TestContainer *c = g->getContainer(i);
            if (c != NULL)
            {
                // Execute runner for this group
                Testing::TestStatus result = c->accept(this);

                if (result == Testing::kTestFailed)
                {
                   failedTests ++;
                   finalResult = Testing::kTestFailed;
                }
            }
            
        }
        // Signal to output that processing of this group has finished.
        m_io->EndGroup();
        return(finalResult);
      }


}