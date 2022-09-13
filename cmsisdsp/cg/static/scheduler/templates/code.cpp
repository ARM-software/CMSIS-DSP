{% extends "commonc.cpp" %}
/*

Generated with CMSIS-DSP Compute Graph Scripts.
The generated code is not covered by CMSIS-DSP license.

The support classes and code is covered by CMSIS-DSP license.

*/

{% if config.dumpFIFO %}
#define DEBUGSCHED 1
{% endif %}

#include "arm_math.h"
#include "custom.h"
#include "GenericNodes.h"
#include "AppNodes.h"
#include "scheduler.h"

{% macro optionalargs() -%}
{% if config.cOptionalArgs %},{{config.cOptionalArgs}}{% endif %}
{% endmacro -%}

{% block schedArray %}
{% endblock %}

{% block scheduleLoop %}
{% if config.debug %}
    while((cgStaticError==0) && (debugCounter > 0))
{% else %}
    while(cgStaticError==0)
{% endif %}
    {
       /* Run a schedule iteration */
{% for s in schedule %}
       {{nodes[s].cRun()}}
       CHECKERROR;
{% if config.dumpFIFO %}
{% for fifoID in sched.outputFIFOs(nodes[s]) %}
       std::cout << "{{nodes[s].nodeName}}:{{fifoID[1]}}" << std::endl;
       fifo{{fifoID[0]}}.dump();
{% endfor %}
{% endif %}
{% endfor %}

{% if config.debug %}
       debugCounter--;
{% endif %}
       nbSchedule++;
    }
    
{% endblock %}