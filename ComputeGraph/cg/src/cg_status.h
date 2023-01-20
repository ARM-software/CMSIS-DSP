#ifndef _CG_STATUS_H_


 typedef enum
  {
    CG_SUCCESS                    =  0, /**< No error */
    CG_BUFFER_UNDERFLOW           = -1, /**< FIFO underflow */
    CG_BUFFER_OVERFLOW            = -2, /**< FIFO overflow */
    CG_MEMORY_ALLOCATION_FAILURE  = -3, /**< Memory allocation failure */
    CG_INIT_FAILURE               = -4, /**< Node initialization failure */
    CG_SKIP_EXECUTION             = -5, /**< Skip node execution (asynchronous mode) */
    CG_BUFFER_ERROR               = -6, /**< Stop execution due to FIFO overflow or underflow (asynchronous mode for pure function) */
    CG_OS_ERROR                   = -7  /**< RTOS API error */
  } cg_status;



#endif /* _CG_STATUS_H_ */