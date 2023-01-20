{% extends "commonc.cpp" %}

{% block schedArray %}
/*

Description of the scheduling. 

*/
static unsigned int schedule[{{schedLen}}]=
{ 
{{schedDescription}}
};
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
        for(unsigned long id=0 ; id < {{schedLen}}; id++)
        {
            {% if config.eventRecorder -%}
            EventRecord2 (Evt_Node, schedule[id], 0);
            {% endif -%}
            CG_BEFORE_NODE_EXECUTION;

            {% if config.asynchronous -%}
            cgStaticError = 0;
            switch(schedule[id])
            {
                {% for nodeID in range(nbNodes) -%}
                case {{nodeID}}:
                {
                    {% if not nodes[nodeID].isPureNode -%}
                    cgStaticError = {{nodes[nodeID].nodeName}}.prepareForRunning();
                    {%- else -%}
                    {{nodes[nodeID].cCheck(config.asyncDefaultSkip)}}
                    {%- endif %}

                }
                break;

                {% endfor -%}

                default:
                break;
            }

            if (cgStaticError == CG_SKIP_EXECUTION_ID_CODE)
            { 
              cgStaticError = 0;
              continue;
            }

            {% if config.eventRecorder -%}
            if (cgStaticError<0)
            {
                EventRecord2 (Evt_Error, cgStaticError, 0);
            }
            {% endif -%}

            CHECKERROR;

            {% endif -%}

            switch(schedule[id])
            {
                {% for nodeID in range(nbNodes) -%}
                case {{nodeID}}:
                {
                   {{nodes[nodeID].cRun()}}

                   {%- if config.dumpFIFO %}
                   {%- for fifoID in sched.outputFIFOs(nodes[nodeID]) %}
                   
                   std::cout << "{{nodes[nodeID].nodeName}}:{{fifoID[1]}}" << std::endl;
                   fifo{{fifoID[0]}}.dump();
                   {%- endfor %}
                   {%- endif %}

                }
                break;

                {% endfor -%}
                default:
                break;
            }
            CG_AFTER_NODE_EXECUTION;
            {% if config.eventRecorder -%}
            if (cgStaticError<0)
            {
                EventRecord2 (Evt_Error, cgStaticError, 0);
            }
            {% endif -%}
            CHECKERROR;
        }
{% if config.debug %}
       debugCounter--;
{% endif %}
       CG_AFTER_ITERATION;
       nbSchedule++;
    }

{% endblock %}