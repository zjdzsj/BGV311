;/*******************************************************************************
;
;    Copyright(C) 2013-2014 SUPERFLY TECHNOLOGY All rights reserved.
;
;FILE        : option.asm
;AUTHOR      : 王希炳(wxb)(hilbertwang@superfly.com.cn)
;VERSION     : 1.00
;DATE        : 2013/01/01
;DESCRIPTION : 数据类型定义
;CHIP        : RL78/R7F0C019
;HISTORY     :
;    <author>    <time>      <version>   <desc>
;    wxb         13/01/01    1.00        create this file 
;
;DISCLAIMER  :
;本程序所有权归作者所有，
;本程序仅用于实现特定产品的功能，任何修改或其它目的的应用均不作保证，
;任何人未经作者同意不得将本程序的全部或部分用于商业目的  
;
;*******************************************************************************/

OPT CSEG  OPT_BYTE


;/*******************************************************************************
; 常用选项
;*******************************************************************************/
; 系统内部振荡器时钟
OCO_CLK         EQU     4000000

; FLASH 操作模式
CLK_CMODE_LV    EQU     0   ;LV(低电压主)模式
CLK_CMODE_LS    EQU     2   ;LS(低速主)模式
CLK_CMODE_HS    EQU     3   ;HS(高速主)模式
CLK_MODE        EQU     CLK_CMODE_LS

; 看门狗使能
; 0 - 停止，1 - 使能
WDT_ENABLE      EQU     1

; 看门狗定时器的溢出时间
WDT_TIME_4S     EQU     7 ; 7 - 2^16/fIL(3799.19 ms)
WDT_TIME_1S     EQU     6 ; 6 - 2^14/fIL(949.80 ms)
WDT_TIME_0_5S   EQU     5 ; 5 - 2^13/fIL(474.90 ms)
WDT_TIME        EQU     WDT_TIME_4S

; 使用看门狗定时器的间隔中断，达到75% + 1/2fIL溢出时间中断
; 0 - 不使用，1 - 使用
WDT_USE_INT     EQU     0


; 片上调试功能使能
ON_CHIP_DEBUG   EQU     1

; 验证安全ID失败时是否擦除闪存数据: 0 - 擦除，1 - 不擦除
OCDERSD         EQU     0
;/*******************************************************************************
;*******************************************************************************/




;/*******************************************************************************
; 看门狗
;*******************************************************************************/
; 看门狗定时器操作
WDTON       EQU     WDT_ENABLE SHL 4
; 控制HALT/STOP模式时的看门狗定时器操作 
; 0 - 停止，1 - 使能
WDSTBYON    EQU     1 SHL 0
; 使用看门狗定时器的间隔中断，达到75% + 1/2fIL溢出时间中断
WDTINIT     EQU     WDT_USE_INT SHL 7
; 看门狗定时器的溢出时间
WDCS        EQU     WDT_TIME_4S SHL 1

; 看门狗定时器的窗口开启期间为100% 
WDTWINDOW   EQU     3 SHL 5

OB_WDT      EQU     WDTINIT OR WDTWINDOW OR WDTON OR WDCS OR WDSTBYON


;/*******************************************************************************
; LVD
;*******************************************************************************/
;OB_LVD      EQU     0FFH    ; 不使用
;OB_LVD      EQU     073H    ; 复位模式 电压4.0V 9 3.98V
OB_LVD      EQU     071H    ; 中断模式 电压4.0V 9 3.98V
;OB_LVD      EQU     031H    ; 中断模式 电压3.13V 9 3.06V
;/*******************************************************************************
; 时钟
;*******************************************************************************/
; FLASH操作模式
; 0 - LV，2 - LS，3 - HS
CMODE     EQU     CLK_MODE SHL 6

$_IF(1000000 = OCO_CLK)
    FRQSEL      EQU     0DH
$_ELSEIF(2000000 = OCO_CLK)
    FRQSEL      EQU     0CH
$_ELSEIF(4000000 = OCO_CLK)
    FRQSEL      EQU     0BH
$_ELSEIF(8000000 = OCO_CLK)
    FRQSEL      EQU     0AH
$_ELSEIF(16000000 = OCO_CLK)
    FRQSEL      EQU     09H
$_ELSEIF(3000000 = OCO_CLK)
    FRQSEL      EQU     03H
$_ELSEIF(6000000 = OCO_CLK)
    FRQSEL      EQU     02H
$_ELSEIF(12000000 = OCO_CLK)
    FRQSEL      EQU     01H
$_ELSEIF(24000000 = OCO_CLK)
    FRQSEL      EQU     00H
$ELSE
    The value of OCO_CLK(on-chip oscillator clock) is invaild.
$ENDIF
OB_CLOCK      EQU     CMODE OR 030H OR FRQSEL

;/*******************************************************************************
; On-chip debug 调试
;*******************************************************************************/
OCDENSET        EQU     ON_CHIP_DEBUG SHL 7
$_IF(ON_CHIP_DEBUG)
    OB_DEBUG    EQU     OCDENSET OR 04H OR OCDERSD
$ELSE
    OB_DEBUG    EQU     004H;OCDENSET OR OCDERSD
$ENDIF

;/*******************************************************************************
; 选项字节的定义
;*******************************************************************************/
; 看门狗设置
DB OB_WDT

; LVD设置      
DB OB_LVD

; 时钟设置
DB OB_CLOCK

; 片上调试功能设置
DB OB_DEBUG

END

