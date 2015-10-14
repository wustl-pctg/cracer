#ifndef _OMRD_H
#define _OMRD_H

extern "C" void cilk_tool_print(void);
extern "C" void cilk_tool_destroy(void);
extern "C" void __om_enable_checking(); 
extern "C" void __om_disable_checking();
extern "C" void __om_disable_instrumentation();
#endif // _OMRD_H
