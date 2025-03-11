/*
 * usart_bootloader.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Batuhan URAL
 */

#include <string.h>
#include "main.h"

typedef void (*pFunction)(void);
#define APP_ADDRESS  0x08008000  // User_app flash adresi



void Jump_To_Application(void) {
    uint32_t JumpAddress;
    pFunction Jump_To_App;

    // User_APP stack pointer adresini al
    uint32_t app_stack_pointer = *(volatile uint32_t*)APP_ADDRESS;

    // Eğer uygulama yüklü değilse çık
    if (app_stack_pointer < 0x20000000 || app_stack_pointer > 0x2004FFFF) {
        return;
    }

    // Uygulamanın reset vektörünü al
    JumpAddress = *(volatile uint32_t*) (APP_ADDRESS + 4);
    Jump_To_App = (pFunction) JumpAddress;

    // Kesme vektör tablosu başlangıç adresini güncelle
    SCB->VTOR = APP_ADDRESS;

    // Stack pointer'ı uygula
    __set_MSP(app_stack_pointer);

    // Uygulamaya atla
    Jump_To_App();
}

