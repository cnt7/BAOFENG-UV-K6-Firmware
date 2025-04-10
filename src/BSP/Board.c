#include "includes.h"

__IO STR_INTFUN UserVectors[10] __attribute__((at(0x20001000))) __attribute__((section(".intfun")));

//定时1ms
void SysTick_Init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
}

void Gpio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1, ENABLE); 
    
    //初始化GPIO A 输出相关PIN
    // GPIO_InitStructure.GPIO_Pin   = 0X791C;// 0b0111 1001 0001 1100
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;

    // Enabled SWD
    // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 ;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Write(GPIOA,0X0104);

    //初始化GPIOA 输入上拉相关PIN
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化ADC使用的GPIO
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    //初始化SPI1接口
    GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    //初始化GPIO B 输出相关PIN
    //GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = 0xBFFD; // 0b1011 1111 1111 1101
    // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0  | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 
    // | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13  | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Write(GPIOB,0x1C19);// 0b0001 1100 0001 1001
    // GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
    
    //初始化GPIO B 输入相关PIN
    GPIO_InitStructure.GPIO_Pin   = 0X4002; // 0b0100 0000 0000 0010
    // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //初始化SPI2接口
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

    //初始化SPI2接口
    GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //初始化GPIO C 输出相关PIN
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //初始化GPIOC 输入上拉相关PIN
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //初始化GPIO F 输出相关PIN
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_Write(GPIOF, 0X0000);

    //初始化GPIOF 输入上拉相关PIN
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

extern void GpioModeSwitch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if(mode)
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    }
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the USART1 interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure the DMA interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Usart_Init()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Connect PXx to USARTx_Tx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    
    /* Connect PXx to USARTx_Rx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_1);
    
    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);    
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}

extern void uartSendChar( U8 ch )
{
    USART1->TDR = (ch & (uint16_t)0x01FF);
    while ((USART1->ISR & USART_FLAG_TXE) == (uint16_t)RESET); 
}


void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);   
    
    SPI_I2S_DeInit(SPI2);

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 0;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
}

uint8_t UserADC_GetValOfBatt(void)
{
    uint8_t adcDat;

    ADC1->CHSELR = 0;
    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);
    ADC_StartOfConversion(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    adcDat = (ADC_GetConversionValue(ADC1) >> 4);
	return adcDat;
}

uint8_t UserADC_GetValOfVox(void)
{
    uint8_t adcDat;

    ADC1->CHSELR = 0;
    ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);
    ADC_StartOfConversion(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    adcDat = (ADC_GetConversionValue(ADC1) >> 4);
    return adcDat;
}

void UserADC_Init(void)
{
	ADC_InitTypeDef     ADC_InitStructure;

    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  	/* ADC1 DeInit */  
    ADC_DeInit(ADC1);
    
    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);

    /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure); 
    
    /* Convert the ADC1 Channel0 and channel1 with 55.5 Cycles as sampling time */ 
    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);

    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);
    
    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);     
    
    /* Wait the ADRDY flag */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
    
    /* ADC1 regular Software Start Conv */ 
    ADC_StartOfConversion(ADC1);

    UserADC_GetValOfBatt();
}


void USART1_Handler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        CheckProgromMode( USART_ReceiveData(USART1));
    }
}

void DMA1Int_Handler(void)
{
    if(DMA_GetITStatus(DMA1_IT_GL5))
    {     
        DMA_ClearFlag(DMA1_FLAG_GL5);

        VoiceOutput_Interrupt(); 
    }
}

extern void Board_Init(void)
{
    UserVectors[0].intHandle = SysTickHandler;
    UserVectors[1].intHandle = USART1_Handler;
    UserVectors[3].intHandle = DMA1Int_Handler;

    SysTick_Init();
    NVIC_Configuration();
    Gpio_Init();
    SPI2_Init();
    Usart_Init();
    UserADC_Init();
    SpiFlash_Init();
    AudioHard_Init();

    UserADC_GetValOfBatt();

    SC5260_Init();
    __enable_irq();
}

