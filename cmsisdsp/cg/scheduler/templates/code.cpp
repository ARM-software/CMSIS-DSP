{% extends "commonc.cpp" %}

{% block schedArray %}
{% endblock %}

{% block scheduleLoop %}
    CG_BEFORE_SCHEDULE;
{% if config.debug %}
    while((cgStaticError==0) && (debugCounter > 0))
{% else %}
    while(cgStaticError==0)
{% endif %}
    {
       /* Run a schedule iteration */
       {% if config.eventRecorder -%}
       EventRecord2 (Evt_Scheduler, nbSchedule, 0);
       {% endif -%}
       CG_BEFORE_ITERATION;
{% for s in schedule %}
       {% if config.eventRecorder -%}
       EventRecord2 (Evt_Node, {{nodes[s].codeID}}, 0);
       {% endif -%}
       CG_BEFORE_NODE_EXECUTION;
       {{nodes[s].cRun()}}
       CG_AFTER_NODE_EXECUTION;
       {% if config.eventRecorder -%}
       if (cgStaticError<0)
       {
           EventRecord2 (Evt_Error, cgStaticError, 0);
       }
       {% endif -%}
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
       CG_AFTER_ITERATION;
       nbSchedule++;
    }
    
{% endblock %}