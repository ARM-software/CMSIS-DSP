/*

Generated with CMSIS-DSP Compute Graph Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/

#ifndef _SCHED_H_ 
#define _SCHED_H_

{% macro optionalargs() -%}
{% if config.cOptionalArgs %},{{config.cOptionalArgs}}{% endif %}
{% endmacro -%}

#ifdef   __cplusplus
extern "C"
{
#endif

{% if config.eventRecorder %}
#include "EventRecorder.h"

#define EvtSched 0x01 

#define Evt_Scheduler   EventID (EventLevelAPI,   EvtSched, 0x00)
#define Evt_Node        EventID (EventLevelAPI,   EvtSched, 0x01)
#define Evt_Error       EventID (EventLevelError,   EvtSched, 0x02)

{% endif %}

extern uint32_t {{config.schedName}}(int *error{{optionalargs()}});

#ifdef   __cplusplus
}
#endif

#endif

