/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        Pattern.h
 * Description:  Pattern Header
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
#ifndef _PATTERN_H_
#define _PATTERN_H_

#include "Test.h"
#include "Pattern.h"

namespace Client {

template <typename T> 
T *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples=MAX_NB_SAMPLES)
{
    return(NULL);
};

template <>
float64_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
float32_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
q31_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
q15_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
q7_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
uint32_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
uint16_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <>
uint8_t *loadPattern(Testing::PatternID_t id, PatternMgr *mgr,Testing::nbSamples_t &nb, Testing::nbSamples_t maxSamples);

template <typename T> 
T *localPattern(Testing::nbSamples_t id, PatternMgr *mgr)
{
    return(NULL);
};

template <>
float64_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
float32_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
q31_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
q15_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
q7_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
uint32_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
uint16_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

template <>
uint8_t *localPattern(Testing::nbSamples_t nb, PatternMgr *mgr);

extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t nb,float64_t* data,PatternMgr *mgr);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,float32_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,q31_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,q15_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,q7_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,uint32_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,uint16_t*,PatternMgr *);
extern void dumpPattern(Testing::outputID_t id,Testing::nbSamples_t,uint8_t*,PatternMgr *);



template <class T>
class AnyPattern {
    protected:
       // Pattern data
       T *data;
       // To know if the pattern has loaded any data
       bool isLoaded;
       // Memory generation when the data was loaded.
       // If memory generation is different when accessing the data
       // the pattern should return NULL.
       unsigned long currentGen;
       PatternMgr *m_mgr;
       // Nb of samples in the loaded pattern
       Testing::nbSamples_t m_nbSamples;
    public:
       AnyPattern()
       {
            this->data = NULL;
            this->isLoaded = false;
            this->currentGen = 0;
            this->m_mgr=NULL;
            this->m_nbSamples = 0;
       }

       /** Get pointer to the pattern data.

           Pointer is NULL in following conditions:
             Memory generation of pattern is different from memory manager's one
             Pattern not loaded
             Number of samples i 0

       */
       T *ptr()
       {
          
           if (this->currentGen != this->m_mgr->generation())
           {
              return(NULL);
           }
           else 
           {
              if (this->isLoaded)
              {
                 if (this->m_nbSamples > 0)
                 {
                   return(this->data);
                 } 
                 else
                 {
                   return(NULL);
                 }
              }
              else
              {
                 return(NULL);
              }
           }
       }

       Testing::nbSamples_t nbSamples()
       {
          return(this->m_nbSamples);
       }

};

/** An input pattern

*/
template <class T>
class Pattern : public AnyPattern<T>{
    private:
       Testing::PatternID_t m_id;
    public:
       Pattern()
       {
       }

       /** Reload fresh data for the pattern.

           If memory manager has not released its memory,
           reloading an already loaded pattern will leak some memory
           since the previous pattern will still be allocated
           in the memory manager.

           Generally this reload is used in setUp function of tests.
           The memory being released in the tearDown function.

       */
       void reload(Testing::PatternID_t id,PatternMgr *mgr, Testing::nbSamples_t maxSamples=0)
       {
           Testing::nbSamples_t nbSamples;
           this->m_id = id;
           this->m_mgr=mgr;
           this->currentGen = this->m_mgr->generation();
           this->data = loadPattern<T>(this->m_id,this->m_mgr,nbSamples,maxSamples);
           // Initialize the field with the number of samples read
           // (which may have been constrained with maxSamples)
           this->m_nbSamples = nbSamples;
           this->isLoaded = true;
       }
};

/** An reference pattern

    The difference with input pattern is that reference
    patterns are not loaded in dump mode and are not wasting
    memory.

*/
template <class T>
class RefPattern : public AnyPattern<T>{
    private:
       Testing::PatternID_t m_id;
    public:
       RefPattern()
       {
       }
       void reload(Testing::PatternID_t id,PatternMgr *mgr, Testing::nbSamples_t maxSamples=0)
       {
           Testing::nbSamples_t nbSamples;
           this->m_id = id;
           this->m_mgr=mgr;
           this->currentGen = this->m_mgr->generation();
           // Reference patterns are not loaded in dump mode
           if (this->m_mgr->runningMode() != Testing::kDumpOnly)
           {
             this->data = loadPattern<T>(this->m_id,this->m_mgr,nbSamples,maxSamples);
             this->m_nbSamples = nbSamples;
             this->isLoaded = true;
           }
           else
           {
             this->data=NULL;
             this->m_nbSamples = 0;
             this->isLoaded = false;
           }
       }
};

/** A local pattern is to be used for an output.
    It is the only way for the test to allocate memory in the
    memory manager.

    Local patterns can be dumped.

    A local pattern is not dumped when in test mode.

*/
template <class T>
class LocalPattern : public AnyPattern<T>{
    private:
       Testing::outputID_t m_id;
    public:
       LocalPattern()
       {
       }
       void create(Testing::nbSamples_t nbSamples,Testing::outputID_t id,PatternMgr *mgr)
       {
           this->m_nbSamples = nbSamples;
           this->m_mgr=mgr;
           this->m_id=id;
           this->currentGen = this->m_mgr->generation();
           this->data = localPattern<T>(nbSamples,this->m_mgr);
           this->isLoaded = true;
       }

       void dump(PatternMgr *mgr)
       {
           if (this->m_mgr->runningMode() != Testing::kTestOnly)
           {
              dumpPattern(this->m_id,this->m_nbSamples,this->data,this->m_mgr);
           }
       }
};


}

#endif
