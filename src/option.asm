;/*******************************************************************************
;
;    Copyright(C) 2013-2014 SUPERFLY TECHNOLOGY All rights reserved.
;
;FILE        : option.asm
;AUTHOR      : ��ϣ��(wxb)(hilbertwang@superfly.com.cn)
;VERSION     : 1.00
;DATE        : 2013/01/01
;DESCRIPTION : �������Ͷ���
;CHIP        : RL78/R7F0C019
;HISTORY     :
;    <author>    <time>      <version>   <desc>
;    wxb         13/01/01    1.00        create this file 
;
;DISCLAIMER  :
;����������Ȩ���������У�
;�����������ʵ���ض���Ʒ�Ĺ��ܣ��κ��޸Ļ�����Ŀ�ĵ�Ӧ�þ�������֤��
;�κ���δ������ͬ�ⲻ�ý��������ȫ���򲿷�������ҵĿ��  
;
;*******************************************************************************/

OPT CSEG  OPT_BYTE


;/*******************************************************************************
; ����ѡ��
;*******************************************************************************/
; ϵͳ�ڲ�����ʱ��
OCO_CLK         EQU     4000000

; FLASH ����ģʽ
CLK_CMODE_LV    EQU     0   ;LV(�͵�ѹ��)ģʽ
CLK_CMODE_LS    EQU     2   ;LS(������)ģʽ
CLK_CMODE_HS    EQU     3   ;HS(������)ģʽ
CLK_MODE        EQU     CLK_CMODE_LS

; ���Ź�ʹ��
; 0 - ֹͣ��1 - ʹ��
WDT_ENABLE      EQU     1

; ���Ź���ʱ�������ʱ��
WDT_TIME_4S     EQU     7 ; 7 - 2^16/fIL(3799.19 ms)
WDT_TIME_1S     EQU     6 ; 6 - 2^14/fIL(949.80 ms)
WDT_TIME_0_5S   EQU     5 ; 5 - 2^13/fIL(474.90 ms)
WDT_TIME        EQU     WDT_TIME_4S

; ʹ�ÿ��Ź���ʱ���ļ���жϣ��ﵽ75% + 1/2fIL���ʱ���ж�
; 0 - ��ʹ�ã�1 - ʹ��
WDT_USE_INT     EQU     0


; Ƭ�ϵ��Թ���ʹ��
ON_CHIP_DEBUG   EQU     1

; ��֤��ȫIDʧ��ʱ�Ƿ������������: 0 - ������1 - ������
OCDERSD         EQU     0
;/*******************************************************************************
;*******************************************************************************/




;/*******************************************************************************
; ���Ź�
;*******************************************************************************/
; ���Ź���ʱ������
WDTON       EQU     WDT_ENABLE SHL 4
; ����HALT/STOPģʽʱ�Ŀ��Ź���ʱ������ 
; 0 - ֹͣ��1 - ʹ��
WDSTBYON    EQU     1 SHL 0
; ʹ�ÿ��Ź���ʱ���ļ���жϣ��ﵽ75% + 1/2fIL���ʱ���ж�
WDTINIT     EQU     WDT_USE_INT SHL 7
; ���Ź���ʱ�������ʱ��
WDCS        EQU     WDT_TIME_4S SHL 1

; ���Ź���ʱ���Ĵ��ڿ����ڼ�Ϊ100% 
WDTWINDOW   EQU     3 SHL 5

OB_WDT      EQU     WDTINIT OR WDTWINDOW OR WDTON OR WDCS OR WDSTBYON


;/*******************************************************************************
; LVD
;*******************************************************************************/
;OB_LVD      EQU     0FFH    ; ��ʹ��
;OB_LVD      EQU     073H    ; ��λģʽ ��ѹ4.0V 9 3.98V
OB_LVD      EQU     071H    ; �ж�ģʽ ��ѹ4.0V 9 3.98V
;OB_LVD      EQU     031H    ; �ж�ģʽ ��ѹ3.13V 9 3.06V
;/*******************************************************************************
; ʱ��
;*******************************************************************************/
; FLASH����ģʽ
; 0 - LV��2 - LS��3 - HS
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
; On-chip debug ����
;*******************************************************************************/
OCDENSET        EQU     ON_CHIP_DEBUG SHL 7
$_IF(ON_CHIP_DEBUG)
    OB_DEBUG    EQU     OCDENSET OR 04H OR OCDERSD
$ELSE
    OB_DEBUG    EQU     004H;OCDENSET OR OCDERSD
$ENDIF

;/*******************************************************************************
; ѡ���ֽڵĶ���
;*******************************************************************************/
; ���Ź�����
DB OB_WDT

; LVD����      
DB OB_LVD

; ʱ������
DB OB_CLOCK

; Ƭ�ϵ��Թ�������
DB OB_DEBUG

END

