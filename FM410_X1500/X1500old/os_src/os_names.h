// os_names.h

#ifndef   __OS_NAMES_H__
#define   __OS_NAMES_H__


// function names
// os_core.c
#define  OSEventNameGet                 os_EventGetName                        
#define  OSEventNameSet                 os_EventSetName                        
#define  OSInit                         os_Init                                
#define  OSIntEnter                     os_EnterISR                            
#define  OSIntExit                      os_ExitISR                             
#define  OSSchedLock                    os_ScheduleLock                        
#define  OSSchedUnlock                  os_ScheduleUnlock                      
#define  OSStart                        os_SystemStart                         
#define  OSStatInit                     os_StatisticsInit                      
#define  OSTimeTick                     os_SystemTick                          
#define  OSVersion                      os_Verson                              
#define  OS_Dummy                       os_Dummy                               
#define  OS_EventTaskRdy                os_EventTaskReady                      
#define  OS_EventTaskWait               os_EventTaskWait                       
#define  OS_EventTO                     os_EventTimeout                        
#define  OS_EventWaitListInit           os_EventWaitListInit                   
#define  OS_MemClr                      os_ZeroMemory                          
#define  OS_MemCopy                     os_memcpy                              
#define  OS_Sched                       os_Schedule                            
#define  OS_StrCopy                     os_strcpy                              
#define  OS_StrLen                      os_strlen                              
#define  OS_TaskIdle                    os_TaskIdle                            
#define  OS_TaskStat                    os_TaskStatistic                       
#define  OS_TaskStatStkChk              os_TaskCheckStk                        
#define  OS_TCBInit                     os_InitTCB                             
                                                                                 
// os_flag.c                            
#define  OSFlagAccept                   os_FlagAccept                          
#define  OSFlagCreate                   os_FlagCreate                          
#define  OSFlagDel                      os_FlagDelete                          
#define  OSFlagNameGet                  os_FlagGetName                         
#define  OSFlagNameSet                  os_FlagSetName                         
#define  OSFlagPend                     os_FlagPend                            
#define  OSFlagPendGetFlagsRdy          os_FlagGetCausedFlags                  
#define  OSFlagPost                     os_FlagPost                            
#define  OSFlagQuery                    os_FlagQuery                           
#define  OS_FlagInit                    os_FlagInit                            
#define  OS_FlagTaskRdy                 os_FlagTaskReady                       
#define  OS_FlagUnlink                  os_FlagUnlink                          
                                                                                 
// os_mbox.c                            
#define  OSMboxAccept                   os_MailBoxAccept                       
#define  OSMboxCreate                   os_MailBoxCreate                       
#define  OSMboxDel                      os_MailBoxDelete                       
#define  OSMboxPend                     os_MailBoxPend                         
#define  OSMboxPost                     os_MailBoxPost                         
#define  OSMboxPostOpt                  os_MailBoxPostEx                       
#define  OSMboxQuery                    os_MailBoxQuery                        
                                                                                 
// os_mem.c                             
#define  OSMemCreate                    os_MemoryCreate                        
#define  OSMemGet                       os_MemoryGetBlock                      
#define  OSMemNameGet                   os_MemoryGetName                       
#define  OSMemNameSet                   os_MemorySetName                       
#define  OSMemPut                       os_MemoryReleaseBlock                  
#define  OSMemQuery                     os_MemoryQuery                         
#define  OS_MemInit                     os_MemoryInit                          
                                                                                 
// os_mutex.c                           
#define  OSMutexAccept                  os_MutexAccept                         
#define  OSMutexCreate                  os_MutexCreate                         
#define  OSMutexDel                     os_MutexDelete                         
#define  OSMutexPend                    os_MutexPend                           
#define  OSMutexPost                    os_MutexPost                           
#define  OSMutexQuery                   os_MutexQuery                          
                                                                                 
// os_q.c                                                                        
#define  OSQAccept                      os_QueueAccept                         
#define  OSQCreate                      os_QueueCreate                         
#define  OSQDel                         os_QueueDelete                         
#define  OSQFlush                       os_QueueFlush                          
#define  OSQPend                        os_QueuePend                           
#define  OSQPost                        os_QueuePost                           
#define  OSQPostFront                   os_QueuePostIntoFront                    
#define  OSQPostOpt                     os_QueuePostEx                         
#define  OSQQuery                       os_QueueQuery                          
#define  OS_QInit                       os_QueueInit                           
                                                                                 
// os_sem.c                             
#define  OSSemAccept                    os_SemaphoreAccept                     
#define  OSSemCreate                    os_SemaphoreCreate                     
#define  OSSemDel                       os_SemaphoreDelete                     
#define  OSSemPend                      os_SemaphorePend                       
#define  OSSemPost                      os_SemaphorePost                       
#define  OSSemQuery                     os_SemaphoreQuery                      
#define  OSSemSet                       os_SemaphoreSet                        
                                                                                 
// os_task.c                            
#define  OSTaskChangePrio               os_TaskChangePriority                  
#define  OSTaskCreate                   os_TaskCreate                
#define  OSTaskInfo                     os_TaskInfo
#define  OSTaskCreateExt                os_TaskCreateEx                        
#define  OSTaskDel                      os_TaskDelete                          
#define  OSTaskDelReq                   os_TaskRequestDelete                   
#define  OSTaskNameGet                  os_TaskGetName                         
#define  OSTaskNameSet                  os_TaskSetName                         
#define  OSTaskResume                   os_TaskResume                          
#define  OSTaskStkChk                   os_TaskCheckStack                      
#define  OSTaskSuspend                  os_TaskSuspend                         
#define  OSTaskQuery                    os_TaskQuery                           
#define  OS_TaskStkClr                  os_TaskClearStack                      
#define  OSGetCurrentTaskPrio           os_GetCurrentTaskPrio 
#define  OSGetTaskStackDepth            os_GetTaskStackDepth

// os_time.c                            
#define  OSTimeDly                      os_TimeDelay                           
#define  OSTimeDlyHMSM                  os_TimeDelayHMSM                       
#define  OSTimeDlyResume                os_TimeDelayResume                     
#define  OSTimeGet                      os_TimeGet                             
#define  OSTimeSet                      os_TimeSet                             
                                                                                 
// os_tmr.c                             
#define  OSTmrCreate                    os_TimerCreate                         
#define  OSTmrDel                       os_TimerDelete                         
#define  OSTmrNameGet                   os_TimerNameGet                        
#define  OSTmrRemainGet                 os_TimerGetRemainTime                  
#define  OSTmrStateGet                  os_TimerGetStatus                      
#define  OSTmrStart                     os_TimerStart                          
#define  OSTmrStop                      os_TimerStop                           
#define  OSTmrSignal                    os_TimerSignal                         
#define  OSTmr_Init                     os_TimerInit                           


// struct names
#define  OS_EVENT                       OS_EVENT
#define  OS_FLAG_GRP                    OS_FLAG_GRP
#define  OS_FLAG_NODE                   OS_FLAG_NODE
#define  OS_MBOX_DATA                   OS_MAIL_BOX_DATA
#define  OS_MEM                         OS_MEMORY
#define  OS_MEM_DATA                    OS_MEMORY_DATA
#define  OS_MUTEX_DATA                  OS_MUTEX_DATA
#define  OS_Q                           OS_QUEUE
#define  OS_Q_DATA                      OS_QUEUE_DATA
#define  OS_SEM_DATA                    OS_SEMAPHORE_DATA
#define  OS_STK_DATA                    OS_STACK_DATA
#define  OS_TCB                         OS_TCB


// global variables
#define  OSTCBCur                       g_OS_TCB_Current


#endif    //__OS_NAMES_H__
