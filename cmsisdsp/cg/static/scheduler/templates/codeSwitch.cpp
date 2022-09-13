{% extends "commonc.cpp" %}

{% block schedArray %}
/*

Description of the scheduling. It is a list of nodes to call.
The values are indexes in the previous array.

*/
static unsigned int schedule[{{schedLen}}]=
{ 
{{schedDescription}}
};
{% endblock %}

{% block scheduleLoop %}
{% if config.debug %}
    while((cgStaticError==0) && (debugCounter > 0))
{% else %}
    while(cgStaticError==0)
{% endif %}
    {
        /* Run a schedule iteration */
        for(unsigned long id=0 ; id < {{schedLen}}; id++)
        {
            switch(schedule[id])
            {
                {% for nodeID in range(nbNodes) -%}
                case {{nodeID}}:
                {
                   {{nodes[nodeID].cRun()}}
                   CHECKERROR;
                }
                break;

                {% endfor %}default:
                break;
            }
        }
{% if config.debug %}
       debugCounter--;
{% endif %}
       nbSchedule++;
    }

{% endblock %}