// --- Ejemplo de parpadeo de LED LD2 en STM32F476RGTx -------------------------
#include <stdint.h>

#define WFI()       __asm volatile("wfi")

void init_systick(void);
void init_led(void);

// --- Definiciones de registros para LD2 (Ver RM0351) -------------------------
#define RCC_BASE    0x40021000                         // Reset and Clock Control base
#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_BASE  + 0x4C)) // Habilita GPIOA clock

#define GPIOA_BASE  0x48000000                                  // GPIOA base
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+ 0x00))  // Configuración de modo
#define GPIOA_ODR   (*(volatile uint32_t *)(GPIOA_BASE+ 0x14))  // Data de salida
#define LD2_PIN     5                                           // Pin PA5 (LED)

// --- Definiciones de registros para SysTick (Ver PM0214)  --------------------
#define SYST_BASE   0xE000E010                                  // SysTick base
#define SYST_CSR    (*(volatile uint32_t *)(SYST_BASE + 0x00))  // Control y estado
#define SYST_RVR    (*(volatile uint32_t *)(SYST_BASE + 0x04))  // Valor de recarga
#define HSI_FREQ    4000000                                     // Reloj interno 4 MHz

// --- Programa principal ------------------------------------------------------
int main(void)
{
    init_led();
    init_systick();
    while (1) {
        WFI();  // Espera interrupción
    }
}

// --- Inicialización de GPIOA PA5 para el LED LD2 -----------------------------
void init_led(void)
{
    RCC_AHB2ENR |= (1 << 0);                      // Habilita reloj GPIOA
    GPIOA_MODER &= ~(3 << (LD2_PIN*2));           // Limpia bits
    GPIOA_MODER |=  (1 << (LD2_PIN*2));           // Configura como salida
}

// --- Inicialización de Systick para 1 s --------------------------------------
void init_systick(void)
{
    SYST_RVR = HSI_FREQ - 1;                      // Recarga = 4000000 - 1
    SYST_CSR = (1 << 0) | (1 << 1) | (1 << 2);    // ENABLE|TICKINT|CLKSOURCE
}

// --- Manejador de la interrupción SysTick ------------------------------------
void SysTick_Handler(void)
{
    GPIOA_ODR ^= (1 << LD2_PIN);                  // Alterna el estado del LED
}
