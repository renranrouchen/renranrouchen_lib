/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      app_config.c
 * Author:        zwxf
 * Email:         zhiwei1007@qq.com
 * DateTime:      2017-05-27 09:06:03
 * ModifiedTime:  2017-05-27 09:06:03
 */
#include <common.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#include <gpio.h>
#endif
//#include <string.h>
#include <cfg_api.h>
#include <log/log.h>
#include <app_config.h>
#include <event_process.h>
#include <cJSON.h>
#include <wifi_clock_cfg.h>
extern sdk_param sdkParam;
#if 1
const char* cityTable [2475] = {
"����","����","˳��","����","ͨ��","��ƽ","����","��̨","ʯ��ɽ",
"����","��ɽ","����","��ͷ��","ƽ��","����","���","����","����","����","����","����",
"����","����","��������","����","����","�Ϻ�","��ɽ","�ζ�","�ֶ�����","����","�ɽ�",
"����","����","���","����","��ɽ","ʯ��ׯ","�żҿ�","�е�","��ɽ","�ػʵ�","����",
"��ˮ","��̨","����","����","�ȷ�","֣��","����","����","ƽ��ɽ","����","����","����",
"����","����","����","�ױ�","���","�ܿ�","���","פ����","����Ͽ","��Դ","����","�Ϸ�",
"�ߺ�","����","����ɽ","����","����","����","����","��ɽ","����","����","ͭ��","����",
"����","����","����","����","��ɽ","����","����","��","����","̨��","����","��ˮ","����",
"����","����","�ϴ�","�ϴ�","����","�山","����","����","����","ǭ��","����","����","����",
"�ǿ�","����","��Ϫ","���","��ɽ","����","�潭","��ƽ","����","ʯ��","����","�ٲ�","ͭ��",
"�ɽ","�ᶼ","��¡","��ˮ","�뽭","����","��ɽ","����","����","Ȫ��","����","����","����",
"��ƽ","����","����","����","����","����","ƽ��","����","����","���","������","��Ȫ",
"��ˮ","�䶼","����","����","����","����","��Ҵ","����","����","÷��","��ͷ","����","�麣",
"��ɽ","����","տ��","����","��Դ","��Զ","�Ƹ�","����","��ݸ","��ɽ","����","����","ï��",
"��β","�ع�","����","����","����","����","����","���Ǹ�","���","����","��ɫ","����",
"�ӳ�","����","����","����","����","��˳","����","ͭ��","�Ͻ�","����ˮ","����","����",
"����","���","��ɽ","��Ϫ","����","�ն�","��ͨ","�ٲ�","ŭ��","�������","����","�º�",
"����","����","����","��ɽ","���ͺ���","�ں�","����","ͨ��","����������","������˹",
"�ٺ�","����","����","��ͷ","���","�ϲ�","����","����","�˴�","ӥ̶","����",
"������","Ƽ��","����","�Ž�","����","�人","�Ƹ�","����","�˲�","��ʩ","ʮ��","��ũ��",
"����","����","����","����","Ǳ��","����","����","Т��","��ʯ","����","�ɶ�","�Թ�","����",
"�ϳ�","����","����","�㰲","����","����","�˱�","�ڽ�","����","��ɽ","üɽ","��ɽ","�Ű�",
"����","����","����","��Ԫ","��֦��","����","����","��ԭ","ʯ��ɽ","����","����","����",
"����","����","����","����","����","����","����","Ϋ��","����","����","����","��Ӫ","����",
"��ׯ","����","����","�ĳ�","�ൺ","�Ͳ�","����","��̨","����","̩��","����","�Ӱ�","����",
"ͭ��","����","����","����","����","����","μ��","̫ԭ","�ٷ�","�˳�","˷��","����","����",
"��ͬ","��Ȫ","����","����","����","��³ľ��","ʯ����","����","��³��","�����","������",
"������","��ʲ","����","����","����","����","����̩","��ͼʲ","����","��������","����",
"ɽ��","����","����","����","�տ���","��֥","̨����","����","̨��","����","����","����",
"�ٸ�","����","����","����","��ɳ","����","����","�Ͳ�","����","�Ĳ�","��ͤ","����","��ˮ",
"�ֶ�","��ָɽ","��ɳ","����","����","����","����","����","¦��","����","����","�żҽ�",
"����","����","����","��̶","�Ͼ�","��","����","��ͨ","����","��Ǩ","����","����",
"���Ƹ�","����","̩��","����","�γ�","������","ĵ����","��ľ˹","�绯","�ں�","˫Ѽɽ",
"����","����","��̨��","����","�׸�","�������","���˰���","����","�Ӽ�","��ƽ","��ɽ",
"�׳�","��Դ","��ԭ","����","ͨ��","����","��ɽ","��˳","��Ϫ","����","��«��","Ӫ��",
"����","����","����","����","�̽�","����","����","����","����","����","����ɽ","����",
"����","������","������","����","����","������","������","����","����������","��½","����",
"��ƽ","����","����","����","��ͼ","������","����","��Ϫ","����","����","����","��Զ",
"����","����","����","����","����","����","�ݳ�","�ͳ�","����","�Ͷ�","������","����",
"���","�ո�","����","�׺�","����","����","����","����","����","ľ��","������","��������",
"��������","����","����","����","��Ʊ","����","��Ȫ","��ˮ","��ͷ","����","����","����",
"����","����","��Ϫ��","����","����","���ƶ���","��Ӧ","����","����","����","����",
"�첼���","���","�°Ͷ�����","�ǲ�","���","�γ�","����","�̵�","����","����","᯹�",
"�ʹ�","�ǹ�","����","���","���������ǰ��","�������������","�������������","�ӻ�",
"����","�ӽ�","�ν�","����","����","����","����","����","����","����","����","����",
"����","����","����","����","��ƽ","����","����","��������","����","��ɽ","��ˮ","��˳",
"��̩","��͡","��ͼ","����","����","����","����","����","��Ϫ","��Ϫ","�Ϫ","����","����",
"��Ϫ","����","����","����","����","����","��ԫ","��Դ","����","����","��","�°�","����",
"����","���","�±�","����","����","���","����","����","����","���","��","�²�",
"�Ŷ�����","��","����","���","�Ƿ�","����","����","����","���","����",
"�κ�","�»�","�ػ�","�»�","�ػ�","�½�","������","������","���","����","����","�������",
"����","����","������˹������","����","����","����","����","����","����","��ƽ","����",
"����","����","����","����","����","����","��ʤ","��ɽ","�ɽ","��ɽ","��ɽ","��ʯ��",
"����","����","��̨","����","��ͷ","��ͨ","��Ϳ","����","���ֹ��ն�����","����","����",
"����","����","����","����","����","����","����","����","��Ҧ","��ұ","����","��Զ","����",
"����","����","����","����","����","��կ","���","�������˶����","��������","����",
"��üɽ","��ƽ","��ɽ","������˹���п�","������˹��ǰ��","��Դ","����","���","�ʳ�","����",
"���","����","����","����","�ʶ�","����","���","���","����","����","����","����","�",
"���","����","�⿪","����","����","����","����","����","����","����","����","��ƽ","��ƽ",
"��ƺ","����","����","����","��Ȫ","����","��Զ","��ɽ","��ɽ","��˳","��ɽ","��˳","����",
"��̨","����","����","����","����","����","����","����","����","����","����","����","����",
"����","����","����","��ԣ","����","��خ","��Դ","����","����","����","����","�߰�","����",
"�̰�","�߱���","޻��","�ߴ�","�ȳ�","����","�ղ�","���","���","���","�ʵ�","��","��",
"���","�ʹ�","�㺺","����","���","����","�Ѿ�","����","ׯ��","����","����","����","����",
"����","����","����","����","����","����","����","����","����","��ƽ","��ƽ","��ƽ","����",
"��Ȫ","����","��ˮ","��ʼ","��ɽ","��ɽ","����","��̨","����","����","��Ϫ","����","����",
"Ǭ��","����","����","����","����","��Ҫ","����","����","����","����","��Դ","����","����",
"����","����","����","����","����","����","����","������","����","����","����","�찲",
"�Ͳ�������","���ͺ�","����","�봨","����","����","���","����","����","����","����",
"�괨","����","���","�ⶫ","�ݶ�","�ᶫ","����","���","�׷�","����","����","��ԫ","����",
"���","�Ӽ�","�ӽ�","���","����","����","�Ͻ�","�;�","��������","����","�ӿ�","����",
"����","����","����","������","���ͺ��غ���","����","����","����","����","����","����",
"��¡","����","����","����","��÷","����","����","����","����","����","����","����","����",
"��ũ","��ƽ","��ƽ","��ƺ","����","����","����","����","����","����","����","����","��ɽ",
"��˳","��ɽ","��ɽ","����","��ɽ","��ˮ","��ɽ","��ˮ","��˶","��̨","��ͼ��","��ͤ","�鶴",
"��ͬ","����","����","����","����","����","����","����","����","����","����","����","����",
"����","����","����","����","��Դ","��Զ","��Դ","��ԭ","��Դ","����","����","����","����",
"����","����","����","����","����","����","��ľ��","����","۲��","����","����","����","����",
"캵�","����","����","����","����","����","����ɽ","����","����","�Ժ�","�κ�","����","����",
"����","���","����","����","����","����","�н�","����","����","����",
"����","����","����","����","��ľ����","��ī","�ൺ�Ƶ�","����","����",
"����","����","����","����","���","����","��ƽ","����","�ɽ","����",
"��ɽ","����","��ˮ","��ʼ","��ɽ","��ɳ","��ˮ","٤ʦ","��̨","��̳",
"����","����","��̩","��Ϊ","����","����","����","����","����","����",
"��Ϫ","����","����","��Ϫ","ۣ��","����","����","����","����","����",
"����","����","����","����","����","����","����","��Ұ","����","����",
"����","����","����","����","����","��Զ","��Դ","����","����","��կ",
"����","����","����","����","�⳵","����","����","����","�˶�",
"ͨ�ɿ�������","ͨ�ɿ������","����","����","��","����","ͨ�ɿ���",
"��������","��³","��ƽ","��ƽ","��ƺ","��ɽ","��ɽ","��ʲ������","����",
"��ԭ","��Զ","����","�ٰ�","¡��","����","�鱦","�ܱ�","��","�ײ�",
"����","���","���","º��","�괨","����","����","�质","�ﴨ","����",
"�ֲ�","����","����","�鴨","½��","�޳�","¡��","¤��","�̴�","�崨",
"����","¡��","�ֵ�","�޶�","�޵�","³��","����","�ֶ�","¦��","����",
"½��","»��","᰸�","�ٹ�","�躣","�ٺ�","����","����","����","����",
"�Ϻӿ�","¡��","����","¡��","����","����","�ٽ�","����","����","����",
"����","����","�ֿ�","����","�˿���","����","����","����","���","¡��",
"����","½��","����","��¡","����","����","����","����","����","����",
"��ƽ","��ƽ","��ƽ","��ƽ","��ƺ","����","����","����","����","����",
"��Ȫ","��Ȫ","��Ȫ","µ��","����","��ʯ","��ʯ","��ɽ","����","��ˮ",
"��ˮ","®ɽ","³ɽ","��ɽ","��ɽ","��ɽ","��ɽ","��ʤ","��ɽ","��ˮ",
"��ɽ","¬��","«ɽ","��ˮ","��ˮ��","���","��̨","��̶","��̨","����",
"��ͤ","����","����","����","����","���","����","���","����","����",
"��Ϫ","����","��Ϫ","����","���","��Ϫ","â��","����","¤��","¤��",
"����","����","»Ȱ","����","����","����","���","����","����","��ҵ",
"���","¹��","���","����","����","��Դ","�Դ","��Դ","��Դ","����",
"��Դ","����","����","����","����","����","¹կ","����","��֦","����",
"����","¡��","����","����","����","�ɳ�","���","�ų�","�崨","Ĳ��",
"�ֶ�","���","������","���","�����","����","����","����","�º�",
"���","Į��","÷�ӿ�","�齭","ī��","â��","ľ��","����","����","����",
"ľ��","����","����","ƽ��","����","������","Ī��������","����","����",
"����˹","Ĳƽ","����","����","��Ȩ","����","��ɽ","��ˮ","��ɽ","��ɽ",
"��ɽ","��̶","��Ϫ","����","ü��","���","ï��","����","ã��","����",
"����","��Դ","ī��","��֬","����","����","������","����","ũ��","�ϰ�",
"�ϰ�","����","�ϲ�","����","�ϳ�","�ϵ�","����","�Ϸ�","�Ϲ�","����",
"����","�Ϻ�","ګ��","����","�ڻ�","�ϻ�","�۽�","����","�Ͼ�","�Ͻ�","�Ͻ�","�Ͽ�","����","����","����","����","���տ�","����ľ","����","��ľ��","������","��ľ","����","��ǿ","����","����","����","����","����","����","��Ϫ","��Ϫ","����","����","��Ӻ","��Զ","����","����","��֣","�Ͱ�","�","�հ�","ƽ��","�ֱ�","����","�ֳ�","ƽ��","�ѳ�","ƽ��","�ն�","ƽ��","ƫ��","ƽ��","ƽ��","�ֽ�","ƽ��","�ѽ�","����","����","������","ƽ��","ƽ��","ƽ½","ƽ��","����","ƽ��","����","ƽȪ","��ʯ","��ɽ","ƽɽ","ƽ˳","��ɽ","Ƥɽ","��ɽ","ƽ̶","ƽ��","����","ƽ��","����","����","ƾ��","ۯ��","��Ϫ","����","ƽң","ƽ��","����","����","ƽ��","ƽ��","ƽԭ","ƽԶ","ƽ��","����","����","����","�찲","Ǭ��","�ذ�","Ǩ��","��","ޭ��","�ന","����","�","���","����",
"���","���","ȫ��","����","�ɼ�","�彧","����","����","����","��¡","����","������",
"��ĩ","ȫ��","��ˮ","Ǳɽ","Ǧɽ","�ɽ","��ˮ","ȷɽ","����","��̨","����","��ͭϿ",
"����","Ǩ��","����","����","����","��ϼ","���","���","����","����","����","����","����",
"����","ǧ��","��ԭ","��Է","��Դ","����","��Ԫ","����","����","ȫ��","����","��","�ڰ�",
"�ݳ�","���","���","�綫","���","�ʻ�","�ʻ�","���","�Ž�","����","����","��ƽ","��ƽ",
"����","��Ǽ","��ˮ","����","����","�ǳ�","����","����","����","��Դ","����","˫��","�౱",
"���","ʩ��","�ϲ�","˫��","���","���","���","˳��","ɳ��","�촨","ʯ��","ɯ��","�̳�",
"ɽ��","˳��","���","ʩ��","�۶�","�̶�","����","ʲ��","˫��","��Һ�","�ϸ�","�ٹ�",
"�Ϻ�","����","ɳ��","�̺�","����","���","�Ļ�","����","����","�罭","˫��","����","����",
"˫��","����","˫��","ʯ��","ʯ¥","����","ʯ��","����","ʯ��","��ľ","˼��","����","����",
"���","����","����","����������","����������","˫��","����","˳ƽ","ʯ��","����","ʯ��",
"����","ʯȪ","��ˮ","��ˮ","ʯ��","��ɽ","��ˮ","۷��","����","��˼","����","ʯ̨","��̨",
"����","ɳ��","����","����","����","����","�Ϫ","���","����","��Ϫ","���","����","ʼ��",
"��Ϫ","˫��","����","����","����","����","����","ɽ��","ɳ��","����","ɽ��","����","����",
"��ԭ","����","��־","����","ɣֲ","����","ʦ��","��������","̨��","ͬ��","ͩ��","̫��",
"̫��","۰��","ͩ��","�쳤","�ϳ�","ͨ��","�ڳ�","ͨ��","���","ͬ��","�ﶫ","���","̫��",
"ͭ��","����","ͨ��","�ƺ�","����","ͨ��","̫��","̩��","�ƺ�","̫��","ͨ����","ͬ��","�ҽ�",
"ͨ��","̨��","���","̫��","�ؿ�˹","�п���","�п�ѷ","̩��","����","����","����","����",
"ͩ®","ͼ��","��Ĭ������","��Ĭ������","���","̩��","̫������","̨ǰ","ͻȪ","��ȫ","����",
"��ʲ�����","̩˳","ͨɽ","̨ɽ","��̨","ͨμ","����","̩��","ͩ��","����","ͬ��","ͨ��",
"����","����","��ԭ","ͨ��","��Դ","����","����","����","��ף","ͩ��","�䰲","��","�İ�",
"����","�Ɽ","����","�峣","Χ��","�䴨","���","�ĳ�","�⴨","��","�봨","�������",
"���","�䶨","����","�߷���","���","���","���","�书","���","����","�廪","�޼�","�⽭",
"����","�Ͻ�","�½�","����","����","����","ξ��","������ǰ��","����������","�����غ���",
"����","����","����","����","����","����","��ţ����","��ƽ","����","����","��ǡ","��ȫ",
"��Ȫ","����","��ˮ","΢ɽ","����","ξ��","��ʤ","��ɽ","Ρɽ","��ɽ","��ʲ","����","����",
"ε��","����","����","����","����","ά��","��Ѩ","����","����","��Ӫ","������","����ɽ",
"��ԭ","��Դ","��Դ","��Դ","μԴ","��Զ","����","��կ","����","����","�°�","�°Ͷ�������",
"�°Ͷ�","�²�","�˳�","�²�","���","����","�ó�","����","����","����","���","ϲ��",
"Ѱ��","�¶�","����","����","���","�ŷ�","�̷�","�·�","Ϣ��","�¸�","�˹�","����","�˺�",
"�˺�","����","�ĺ�","�º�","���","�����","����","�˻�","�»�","�»�","�»�","ѭ��","����",
"����","���","�Ľ�","�½�","Ͽ��","�½�","����","С��","�ɾ�","ѷ��","����","����","��¡",
"����","����","����","����","����","����","����","��ƽ","��ƽ","��ƽ","ϼ��","����","����",
"��ˮ","��ˮ","��ɽ","��ˮ","�ˮ","��ɽ","����","ϰˮ","����","��̩","����","����","����",
"����","����","������","Ѱ��","����","����","����","����","����","��Ͽ","����","����",
"����","Ϣ��","����","����","����","����","Ѯ��","��Ұ","Т��","����","����","����","����",
"Ѯ��","����","����","��ԫ","����","��Դ","��֣","����","����","����","����","����","Զ��",
"Ҧ��","�α�","���","����","����","۩��","����","����","����","�ݳ�","����","Ӧ��","�˳�",
"����","����","�ӳ�","�Ӵ�","�˴�","����","�γ�","Ҷ��","�ܴ�","Ӣ��","����","����","����",
"�ڶ�","�˷�","����","����","���","�ຼ","�ĺ�","����","�ƺ�","�غ�","��","�˻�","����",
"����","�཭","�ӽ�","�佭","ӡ��","ӯ��","�ν�","Ԫ��","����","����","���������","Ӣ��ɳ",
"�˾�","����","����ʯ","����","۳��","����","����","����","����","����","��¤","����","����",
"����","ԣ��","Ԫı","����","����","����","����","����","���պ�","ԭƽ","����","��ƽ","����",
"����","����","����","ԫ��","����","����","����","Ԫ��","����","��ˮ","���","��ɽ","��ʦ",
"Ӣɽ","��˳","��ɽ","��˷","Ӫɽ","����","��ɽ","����","��ʤ","��̨","����","��̩","��ͤ",
"����","����","����","����","����","����","Ӧ��","����","����","����","����","����","��Ϫ","����","����","Ҷ��",
"����","����","����","����","Խ��","����","���","��Ҧ","߮��","ԭ��","����",
"Ԫ��","��ԭ","����","��Դ","����","����","����","����","����","����","����",
"گ��","����","��","���","�ű�","�޳�","���","����","�ӳ�","����","�ض�",
"�ζ�","�Ӷ�","���","��","׼�����","����","��","ׯ��","����","��","�޻�",
"մ��","�żҴ�","����","֦��","�ƽ�","�Ͻ�","֯��","�н�","�żҸ�","��","���",
"����","��³","����","������","��¹","��Ĳ","����","����","����","׿��",
"����","��ƽ","��ƽ","��ƽ","��ƽ","��ƺ","����","����","����","��Ȩ","����","����",
"��ɽ","��ɽ","��ˮ","����","����","����","����","��Ϫ","��Ϫ","����","����","����",
"����","����","����","����","����","����","����","մ��","��Դ","����","��Զ","��Դ",
"��Զ","����","��ԭ","����","����","����","����","����","����","׿��","���",
"����","����","����","����","����","��ͨ","����","ǰ��",
"����","��Դ","����","��˳","�±�","����","������","������","����ɽ","����","��ƽ��",
"��ˮ��","���н���","��ï��","��³��","������","������","���͸�","������","���״���",
"���¿���","�������","��������","����ǰ��","¹Ȫ","�ˮ","����","��Ұ","�е���","����",
"��ɽ","����","��Ƥ","����","�ϴ�","����","��ǿ","����","��ǿ","����","�ʳ�","����","����",
"����","�ٳ�","����","����","¡Ң","�Ϻ�","����","��¹","�º�","����","ƽ��","����","���",
"����","����","���","����","�ɰ�","����","����","����","����","κ��","����","����","¬��",
"������","�Ž�","���ƺ��","С����","����","��ͬ��","����","����","����","����","���",
"��ϲ","����","ƽ³","��̨��","����","־��","����","����","����","�²�","ҫ��","����",
"��ɽ","�ʹ�","��ɽ","�ܴ�","����","���","��ɽ","����","�ĵ�","�ٳ�","��ɽ","Ѧ��",
"ỳ�","̨��ׯ","����","����","����","����","��ƽ","����","ݷ��","�����","�ڶ���",
"�׼�̲","�踽","����","������","������˹","����ɽ��","����","����","��������",
"��ˮ","����","ī�񹤿�","����","�ٰ�","����","��¡","����","����","����","лͨ��",
"�ڰ�","����","�Ƕ�","����","�ʲ�","����","����","�Ӳ�","�˶�","ɣ��","����","����",
"����","����","����","����","����","��������","����","ī��","�߰�","������","����",
"����","����","����","����","����","����","����","����","����","˫��","����","����",
"����","�Ｊ","����","����","�ƶ�","��ǫ","���","���","������","���","崲�","����",
"��ʯɽ","ƽ��","�Ͻ�","����","ʯ��","����","����","����","����","�ֿ�","��ɽ","��ͽ",
"����","ͨ��","����","�ζ�","ͭɽ","������","������","���","����","��ԥ","������","����",
"���Ӻ�","Т��","�ŷ�","��ɽ","��½","����ɽ","�˶�","����","é��","����","�޵�","ɳ��",
"��ɽ","����","۴��","����","����","·��","�齭","����","����","®��","�ߺ���","��Ϊ","�˼�",
"��ɽ","����","��ɽ��","��Ϫ","�Ż�ɽ","����","����","����","���","����","�ݰ�","ʯʨ",
"���㵺","�ϲ���","۶��","������","�ְ�","������","����","����","����","��Դ","«Ϫ","�涫",
"�潭����","����","������","����","����","����","��ɽ��","������","����Դ","�з�","�齭",
"��ˮ̲","����","��Ϫ","�ڵ�","����","����","������","�㴨","�컨��","�Ͱ�","ǭ��","����",
"��Ȫ��","��׽�","�ʺ�","����","����","ͨ��","�ﴨ","����","����","�˱���","����","����",
"��","����","����","�Ž�","����","¯��","����","�¸�","����","ʯ��","ɫ��","����","����",
"���","����","����","��կ��","��","��ˮ","����","������","��ԭ","�޽�","䥽�","�佭",
"÷��","����","����","�࿲","��ͷ","ϼɽ","����","�","����","��Դ","����","�ư�","����",
"����","����","�Ҷ�","½��","����","�ӿ�","����","ˮ��","����","��ɽ","�˰�","��ҵ","��",
"����","����","���","����","�½�","����","���е�","·����","��԰","����","����","����",
"̨��","̨��","����","����","�û�","��Ͷ","����","����","����"};
const unsigned int cityIdTable[2475] = 
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,22,23,24,25,26,27,28,30,31,32,34,35,36,37,38,40,
41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,
68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,92,93,94,95,96,
97,98,99,100,101,102,103,104,105,106,107,108,110,111,112,113,114,116,117,118,119,120,
121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,140,141,142,
143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,
164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,
185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,204,205,206,
207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,
228,229,230,231,232,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,
250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,
271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,
292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,
313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,
334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,
355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,
376,377,378,381,382,384,385,386,387,388,389,390,391,392,393,394,396,397,398,399,400,
401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,
422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,
443,444,445,446,447,448,449,450,452,453,454,455,456,457,458,459,460,461,462,463,464,
465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,
486,487,488,489,490,491,492,494,495,496,498,499,500,501,502,503,504,505,506,508,509,
510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,
531,532,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,
553,554,555,556,557,558,559,561,562,563,564,566,567,568,569,570,571,572,573,574,576,
577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,
598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,
619,620,621,622,623,624,625,626,627,628,629,630,631,632,634,635,636,637,638,639,640,
641,642,643,644,645,646,647,648,649,650,651,652,653,655,656,657,658,659,660,661,662,
663,664,665,666,668,669,670,671,672,673,674,675,678,679,680,681,682,683,684,685,686,
687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,
708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,
729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,
750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,770,771,
772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,791,792,793,
794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,
815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,
836,837,838,839,840,841,842,843,845,846,847,848,849,850,851,852,853,854,855,856,857,
858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,
880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,898,899,900,901,
902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,
923,924,925,926,928,929,930,931,932,933,934,935,936,937,938,939,940,942,943,944,945,
946,947,948,949,950,951,952,953,954,956,958,959,960,961,962,963,964,965,966,967,968,
969,970,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,990,991,
992,993,994,995,996,997,998,999,1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,
1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1024,1025,1026,1027,1028,
1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039,1040,1041,1042,1043,1044,1045,
1046,1047,1048,1049,1050,1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1062,1063,
1064,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,
1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,1096,1097,
1098,1099,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,
1115,1116,1117,1118,1119,1120,1121,1122,1123,1124,1125,1126,1127,1128,1129,1130,1131,
1132,1133,1134,1135,1136,1137,1138,1139,1140,1141,1142,1143,1144,1145,1146,1147,1148,
1149,1150,1151,1152,1153,1154,1155,1156,1158,1159,1160,1161,1163,1164,1165,1166,1167,
1168,1170,1171,1172,1173,1174,1175,1176,1177,1178,1179,1180,1181,1182,1183,1184,1185,
1186,1187,1188,1189,1190,1191,1192,1193,1194,1195,1196,1197,1198,1199,1200,1201,1202,
1203,1204,1205,1206,1207,1208,1209,1210,1211,1212,1213,1214,1215,1216,1217,1218,1219,
1220,1222,1223,1224,1225,1226,1227,1228,1229,1230,1231,1232,1233,1234,1235,1236,1238,
1239,1240,1242,1243,1244,1245,1246,1247,1248,1249,1250,1251,1252,1253,1254,1255,1256,
1257,1258,1259,1260,1261,1262,1263,1264,1265,1266,1267,1268,1269,1270,1271,1272,1273,
1274,1275,1276,1277,1278,1279,1280,1281,1282,1283,1284,1285,1286,1287,1288,1289,1290,
1291,1292,1293,1294,1295,1296,1297,1298,1299,1300,1301,1302,1303,1304,1305,1306,1307,
1308,1309,1310,1311,1312,1315,1316,1317,1318,1319,1320,1321,1322,1324,1325,1327,1328,
1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1340,1341,1342,1343,1344,1345,
1346,1347,1348,1349,1350,1351,1352,1353,1354,1355,1356,1357,1358,1360,1361,1362,1363,
1364,1365,1366,1367,1368,1369,1370,1371,1372,1373,1374,1375,1376,1377,1378,1379,1380,
1381,1382,1383,1384,1386,1387,1388,1389,1390,1391,1392,1393,1394,1395,1396,1397,1398,
1399,1400,1401,1402,1403,1404,1406,1407,1408,1409,1410,1411,1412,1413,1414,1415,1416,
1417,1418,1419,1420,1421,1422,1423,1424,1425,1426,1427,1428,1429,1430,1431,1432,1433,
1434,1435,1436,1437,1438,1440,1441,1442,1443,1444,1445,1446,1447,1448,1449,1450,1451,
1453,1454,1455,1456,1457,1458,1459,1460,1461,1462,1463,1464,1465,1466,1467,1468,1469,
1470,1471,1472,1473,1474,1475,1476,1477,1478,1479,1480,1481,1482,1483,1484,1485,1486,
1487,1488,1490,1491,1492,1493,1494,1495,1496,1497,1498,1499,1500,1501,1502,1503,1504,
1506,1507,1508,1509,1510,1511,1512,1514,1515,1516,1517,1518,1519,1520,1521,1522,1523,
1524,1525,1526,1527,1528,1529,1530,1531,1532,1533,1534,1535,1536,1537,1538,1539,1540,
1541,1542,1543,1544,1545,1546,1547,1548,1549,1550,1551,1552,1553,1554,1555,1556,1557,
1558,1559,1560,1561,1563,1564,1565,1566,1567,1568,1569,1570,1571,1572,1573,1574,1575,
1576,1577,1578,1579,1580,1581,1582,1583,1584,1585,1586,1587,1588,1589,1590,1591,1592,
1593,1594,1595,1596,1597,1598,1599,1600,1601,1602,1603,1604,1605,1606,1607,1608,1609,
1610,1611,1613,1614,1615,1616,1617,1618,1619,1620,1621,1622,1623,1624,1625,1626,1627,
1628,1629,1630,1631,1632,1633,1634,1635,1637,1638,1639,1640,1641,1642,1643,1644,1645,
1646,1647,1648,1649,1650,1651,1652,1653,1654,1655,1656,1658,1660,1661,1662,1663,1664,
1665,1666,1667,1668,1669,1670,1671,1672,1673,1674,1675,1676,1677,1678,1679,1680,1681,
1682,1683,1684,1685,1686,1687,1688,1689,1690,1691,1692,1693,1694,1695,1696,1697,1698,
1699,1700,1701,1702,1703,1704,1707,1708,1709,1710,1711,1712,1713,1714,1715,1716,1717,
1718,1719,1720,1721,1722,1723,1724,1725,1727,1728,1729,1730,1731,1732,1733,1734,1735,
1736,1737,1738,1739,1740,1741,1742,1743,1744,1745,1746,1747,1748,1749,1750,1751,1752,
1753,1754,1755,1756,1757,1758,1759,1760,1761,1762,1763,1764,1765,1766,1767,1768,1769,
1770,1771,1772,1773,1774,1775,1776,1777,1778,1779,1780,1781,1782,1783,1784,1785,1786,
1787,1788,1789,1790,1791,1792,1793,1794,1795,1796,1797,1798,1799,1800,1801,1802,1803,
1804,1805,1806,1807,1809,1810,1811,1812,1813,1814,1815,1816,1817,1818,1819,1820,1821,
1822,1823,1824,1825,1826,1827,1828,1829,1830,1831,1832,1833,1834,1835,1836,1837,1838,
1839,1840,1841,1842,1843,1844,1845,1846,1847,1848,1849,1850,1851,1852,1853,1855,1856,
1857,1858,1859,1860,1861,1862,1863,1864,1865,1866,1867,1868,1869,1870,1871,1872,1873,
1874,1875,1876,1877,1878,1879,1880,1881,1882,1883,1885,1886,1887,1888,1889,1890,1891,
1892,1893,1894,1895,1896,1897,1898,1899,1900,1901,1902,1903,1904,1905,1906,1907,1908,
1909,1910,1911,1912,1913,1914,1915,1916,1917,1918,1919,1920,1921,1922,1923,1924,1925,
1926,1927,1928,1929,1930,1931,1932,1933,1934,1935,1936,1938,1939,1940,1941,1942,1943,
1945,1946,1947,1948,1949,1950,1951,1952,1953,1954,1955,1956,1957,1958,1959,1960,1961,
1962,1963,1964,1965,1966,1967,1968,1969,1970,1971,1972,1973,1974,1975,1976,1977,1978,
1979,1980,1981,1982,1983,1984,1985,1986,1987,1988,1989,1990,1991,1992,1993,1994,1995,
1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,
2013,2014,2015,2016,2017,2018,2019,2020,2021,2022,2023,2024,2025,2026,2027,2028,2029,
2030,2031,2032,2033,2034,2035,2036,2037,2038,2039,2040,2041,2042,2043,2044,2045,2046,
2047,2048,2049,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,2061,2062,2063,
2064,2065,2066,2067,2068,2069,2070,2071,2072,2073,2074,2075,2076,2079,2080,2081,2082,
2083,2084,2085,2086,2087,2088,2089,2090,2091,2092,2093,2094,2095,2096,2097,2098,2099,
2100,2101,2102,2103,2104,2106,2107,2108,2109,2110,2111,2112,2113,2114,2115,2116,2117,
2118,2119,2120,2121,2122,2123,2124,2125,2126,2127,2128,2129,2130,2131,2132,2133,2134,
2135,2136,2137,2138,2139,2140,2141,2142,2143,2144,2145,2146,2147,2148,2149,2150,2151,
2152,2153,2154,2155,2156,2157,2158,2159,2160,2161,2162,2163,2164,2165,2166,2167,2168,
2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,
2186,2187,2188,2189,2190,2191,2192,2193,2194,2195,2196,2197,2198,2199,2200,2201,2202,
2203,2204,2205,2206,2207,2208,2209,2210,2211,2212,2213,2214,2215,2216,2217,2218,2219,
2221,2222,2223,2224,2225,2227,2228,2229,2230,2231,2232,2290,2291,2292,2293,2294,2295,2296,2297,2298,2299,2300,2301,2302,2303,
2304,2305,2306,2307,2308,2309,2310,2311,2312,2313,2314,2315,2316,2317,2318,2319,2320,
2321,2322,2323,2324,2325,2326,2327,2328,2329,2330,2331,2332,2333,2334,2335,2336,2338,
2339,2340,2341,2342,2343,2344,2345,2346,2347,2348,2349,2350,2351,2352,2353,2354,2355,
2356,2359,2360,2361,2362,2363,2364,2365,2366,2367,2368,2370,2374,2375,2383,2384,2385,
2386,2387,2388,2389,2390,2391,2392,2393,2394,2396,2397,2398,2399,2400,2401,2402,2403,
2404,2405,2406,2407,2408,2409,2410,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,
2422,2423,2424,2425,2426,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2438,2439,
2440,2441,2442,2443,2445,2446,2449,2450,2451,2452,2453,2454,2455,2456,2457,2458,2459,
2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471,2472,2473,2474,2475,2476,
2477,2478,2479,2480,2481,2482,2483,2484,2485,2487,2488,2489,2490,2491,2493,2494,2495,
2496,2497,2499,2500,2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2512,2513,2515,
2516,2517,2518,2520,2521,2522,2523,2524,2525,2526,2527,2528,2529,2530,2531,2532,2533,
2534,2535,2536,2537,2538,2539,2540,2541,2542,2543,2544,2545,2546,2547,2548,2550,2551,
2552,2553,2554,2555,2556,2557,2558,2559,2560,2561,2562,2563,2564,2565,2566,2567,2568,
2570,2571,2572,2573,2574,2575,2576,2577,2578,2579,2580,2581,2582,2583,2584,2585,2586,
2587,2588,2589,2590,2591,2592,2593,2594,2595,2596,2597,2598,2599,2600,2601,2602,2603,
2604,2605,2606,2607,2608,2609,2610,2611,2613,2615,2616,2617,2618,2619,2620,2621,2622,
2623,2625,2626,2628,2629,2630,2631,2632,2633,2634,2635,2636,2637,2638,2639,2640,2641,
2642,2643,2644,2645,2650};

#if (!WEB_CONFIG && !VERSION_MUSHAN)
#define IS_EQUAL(str,buff)	(strcmp(str,buff) == 0)
typedef enum {
	NOWTIME = 0,
	BRIGHTNESS,
	CITY,
	TEMP,
	ALARMDAY,
	CLOCK1,
	SLEEP1,	
	TIME1,
	REPEAT1,
	CLOCK2,
	SLEEP2,	
	TIME2,
	REPEAT2,
	CLOCK3,
	SLEEP3,	
	TIME3,
	REPEAT3,
	CONFIG_PARAM,
	CLOSE,
	PLUG,
	MAX_INDEX = 255,
}CONFIG_TYPE;

const char* configList[] = {"nowtime","brightness","city","temp","alarmday","clock1","sleep1","time1","repeat1","clock2","sleep2","time2","repeat2","clock3","sleep3","time3","repeat3","flash_param","close","plug"};
enum {
	MONDAY 		= 1 << 0,
	TUESDAY 	= 1 << 1,
	WEDNESDAY	= 1 << 2,
	THURSDAY	= 1 << 3,
	FRIDAY		= 1 << 4,
	SATURDAY	= 1 << 5,
	SUNDAY		= 1 << 6,
};

typedef struct {
	char nowtime;
	char brightness;
	char temp;
	unsigned int city;
	
	char sleep1;
	char sleep2;
	char sleep3;

	char alarm1;
	char alarm2;
	char alarm3;
	alarm_param alm_param[ALARM_NUM];
}t_config_param;
app_config_param appCfgPrm;

static t_config_param configParam;
static void init_socket_param()
{
	char loop = 0;
	memset(&(appCfgPrm.skPam.cltAddr),0,sizeof(struct sockaddr_in));
	memset(&(appCfgPrm.skPam.srvAddr),0,sizeof(struct sockaddr_in));
	for (loop = 0; loop < 10;loop++) {
		appCfgPrm.skPam.clientMsg.clientFd[loop] = -1;
	}
	appCfgPrm.skPam.clientMsg.clientFdIndex = 0;
}

char start_up()
{
	int ret = -1;
	
	appCfgPrm.skPam.srvFd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(appCfgPrm.skPam.srvFd< 0){
		return -1;	
	}
	appCfgPrm.skPam.srvAddr.sin_port = htons(appCfgPrm.skPam.srvPort);
	appCfgPrm.skPam.srvAddr.sin_family = AF_INET;
	appCfgPrm.skPam.srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(appCfgPrm.skPam.srvFd,(struct sockaddr* )&(appCfgPrm.skPam.srvAddr),sizeof(struct sockaddr_in));
	if (ret < 0) {
		return -1;
	}		
	ret = listen(appCfgPrm.skPam.srvFd,4);
	if (ret < 0) {
		return -1;
	}
	return 1;
}

char*
get_post_item(const char* postData,const char* item)
{
    int len = 0;
    char* addr;
    static char str[50];
    addr = (char*)strstr(postData,item);

    if(addr == NULL){
        return NULL;
    }
    addr += strlen(item) + 1;

    for(len;*(addr + len) != '&';len++)
        ;
    memset(str,0,sizeof(str));
    memcpy(str,addr,len);
    return (char* )str;
}

CONFIG_TYPE
get_config_index(const char* buff,char** indexBuff)
{
	unsigned char loop = 0;
	for (;loop < sizeof(configList);loop++) {
		*indexBuff = get_post_item(buff,configList[loop]);
		if (*indexBuff != NULL) {
//			printf("\r\n %s \r\n ",*indexBuff);
			return (CONFIG_TYPE) (loop);
		}
	}
}	

void 
fillin_alarm_param(const char* buff,alarm_param* alm_param,
const char* clock,const char* repeat,const char* sleep,const char* time)
{
	unsigned char loop = 0;
	char dayIndex = 0;
	char getRepeat[10] = {0};
	char getTime[10] = {0};
	char getSleep[10] = {0};
	char* getClock = NULL;
	strcpy(getRepeat,get_post_item(buff,repeat));
	strcpy(getSleep,get_post_item(buff,sleep));
	strcpy(getTime,get_post_item(buff,time));
	getClock = get_post_item(buff,clock);
	if (getClock != NULL) {
//		printf("\r\ngetClock %s getRepeat %s,getSleep %s,getTime %s",getClock,getRepeat,getSleep,getTime);
		if (getRepeat != NULL && !IS_EQUAL("null",getRepeat)) {
			dayIndex = atoi(getRepeat);
//			printf("day index %d",dayIndex);
			for (loop = 0;loop < 7;loop++) {
				if (dayIndex & (1 << loop)) {
					if (IS_EQUAL("true",getClock)) {
						alm_param[loop * 3].isOpen = true;
					} else {
						alm_param[loop * 3].isOpen = false;
					}
					alm_param[loop * 3].isOpenSnooze = IS_EQUAL("true",getSleep);
					alm_param[loop * 3].alarmTime = atoi(getTime);
					alm_param[loop * 3].defaultAlarmTime = atoi(getTime);	
					
				}
			}
		}								
	} 
}

void dump_config_param()
{
	unsigned char alarm_loop = 0;
	printf(" brightness %d, nowtime %d, temp %d, city %d \r\n",
			configParam.brightness,
			configParam.nowtime,
			configParam.temp,
			configParam.city);
	for (;alarm_loop < 21;alarm_loop++) {
	serial_printf(" alram default time: day:%d,default:%d,alarm:%d,snooze:%d,snooze open %d,isopen:%d\n",
					configParam.alm_param[alarm_loop].alarmDay,
					configParam.alm_param[alarm_loop].defaultAlarmTime,
					configParam.alm_param[alarm_loop].alarmTime,
					configParam.alm_param[alarm_loop].snoozeDelay,
					configParam.alm_param[alarm_loop].isOpenSnooze,
					configParam.alm_param[alarm_loop].isOpen);	
		}
}

static void inline plug_switch(unsigned char sw)
{
#define PLUG_SWITCH_PIN 9	
	gpio_enable(PLUG_SWITCH_PIN,1);
	pin_mode(PLUG_SWITCH_PIN,GPIO_OUT);	
	if (sw == true) {
		digital_write(PLUG_SWITCH_PIN,1);
	} else {
		digital_write(PLUG_SWITCH_PIN,0);
	}
}

void parse_get_data(const char* buff,t_config_param* configParam)
{
	unsigned int loop = 0;
	char cityNum[10] = {0};
	CONFIG_TYPE index = MAX_INDEX;
	char* indexBuff = NULL;
	index = get_config_index(buff,&indexBuff);
	switch (index) {
		case BRIGHTNESS:
			configParam->brightness = (char) ((*indexBuff) - '0') - 1;
			break;
		case NOWTIME:
			configParam->nowtime = (strcmp(indexBuff,"on") == 0);
			break;
		case TEMP:
			configParam->temp = (*indexBuff == 'C');
			break;
		case CITY:
			
			configParam->city = cityIdTable[0];
			for (loop = 0;loop < 2475;loop++) {
				if (strncmp(indexBuff,cityTable[loop],strlen(cityTable[loop])) == 0) {
					configParam->city = cityIdTable[loop];
				}
			}
			
			break;
		case CLOCK1:
			fillin_alarm_param(buff,&configParam->alm_param[0],configList[CLOCK1],
								configList[REPEAT1],configList[SLEEP1],configList[TIME1]);
			fillin_alarm_param(buff,&configParam->alm_param[1],configList[CLOCK2],
								configList[REPEAT2],configList[SLEEP2],configList[TIME2]);
			fillin_alarm_param(buff,&configParam->alm_param[2],configList[CLOCK3],
								configList[REPEAT3],configList[SLEEP3],configList[TIME3]);
			break;
		case CLOCK2:
			fillin_alarm_param(buff,&configParam->alm_param[1],configList[CLOCK2],
								configList[REPEAT2],configList[SLEEP2],configList[TIME2]);
			fillin_alarm_param(buff,&configParam->alm_param[2],configList[CLOCK3],
								configList[REPEAT3],configList[SLEEP3],configList[TIME3]);
			break;
		case CLOCK3:
			fillin_alarm_param(buff,&configParam->alm_param[2],configList[CLOCK3],
								configList[REPEAT3],configList[SLEEP3],configList[TIME3]);
			break;
		case CONFIG_PARAM:
			
			sdkParam.nt_param.isOpen = configParam->nowtime;
			//sdkParam.timeFmt_param.fmt = 
			sdkParam.wth_parma.cityId = configParam->city;
			sdkParam.temperature = configParam->temp;
			sdkParam.brightness = configParam->brightness;
			memcpy(sdkParam.alm_param,configParam->alm_param,ALARM_NUM * sizeof(alarm_param));
			
			config_set(&sdkParam);
			config_submit();	
			config_dump();
			reboot(1);
			
			break;
		case CLOSE:
			appCfgPrm.skPam.clientMsg.clientFdIndex -= 1;
			close(appCfgPrm.skPam.clientMsg.clientFd[appCfgPrm.skPam.clientMsg.clientFdIndex]);
			appCfgPrm.skPam.clientMsg.clientFd[appCfgPrm.skPam.clientMsg.clientFdIndex] = -1;
			break;
		case PLUG:
			plug_switch((strcmp(indexBuff,"on") == 0));
			break;
		default:
			break;
	}

}

void
handle_data(char* buff,int len)
{
	printf("%s \r\n",buff);
#if (WIFI_PLUG_DEMO == 0)
	parse_get_data(buff,&configParam);
//	dump_config_param();
#else
	if (strncmp(" plug=",buff,5) == 0) {
		printf("xxxxx");
		if (strncmp("on",(buff + 6),2) == 0) {
			printf("ON");
			plug_switch(true);
		} else {
			printf("OFF");
			plug_switch(false);
		}
	}
#endif
}

void receive_data()
{
	char i = 0;
	char ret = -1;
	char retry = 0;
	unsigned char revBuff[512];
	fd_set fdSet;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	memset(revBuff,0,sizeof(revBuff));
	
	while(1){
		//http_client_select(&fdSet,&appCfgPrm.skPam.clientMsg,handle_data);
		
		FD_ZERO(&fdSet);
		for (i = 0;i < MAX_SERVER_NUM;i++) {
			if (appCfgPrm.skPam.clientMsg.clientFd[i] == -1) {
				sys_msleep(100 * 1);
				continue;
			} else {
				FD_SET(appCfgPrm.skPam.clientMsg.clientFd[i],&fdSet);
				ret = select(appCfgPrm.skPam.clientMsg.clientFd[i] + 1,&fdSet,NULL,NULL,&tv);
				if (ret > 0) {
					memset(revBuff,0,512);
					ret = read(appCfgPrm.skPam.clientMsg.clientFd[i],&revBuff,sizeof(revBuff));	
					//printf("receive data is %s",revBuff);
					if (strcmp(revBuff,"reboot") == 0) {
//						debug("\r\n========starting ota update===========\r\n");
						send_event(REBOOT_EVENT);
					}
					handle_data(revBuff,ret);
				} else {
					;
					//printf("select fail \r\n");
				}
			}
		}
		
	}	
}
void dump_client_message()
{
	printf("clientPort is %d,clientd IP is %s",
			ntohs(appCfgPrm.skPam.cltAddr.sin_port),
			inet_ntoa(appCfgPrm.skPam.cltAddr.sin_addr)
			);
}

static char app_server_select()
{
	char ret = -1;
	fd_set fdSet;
	struct timeval tv;
	FD_ZERO(&fdSet);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_SET(appCfgPrm.skPam.srvFd,&fdSet);

	ret = select(appCfgPrm.skPam.srvFd + 1,&fdSet,NULL,NULL,&tv);
	if(ret < 0){
		printf("\r\n===========socket server select failed=================\r\n");
		return -1;
	}
	if(FD_ISSET(appCfgPrm.skPam.srvFd,&fdSet)){
		return 1;	
	}else{
		return -1;
	}
}
static inline void app_accept()
{ 
	//selet polling
	char ret = -1;
	ret = app_server_select();
	if(ret > 0){
		if(appCfgPrm.skPam.clientMsg.clientFdIndex > 5){
//			assert("\r\n========client number overflow=======\r\n");
			return;
		}
		appCfgPrm.skPam.clientMsg.clientFd[appCfgPrm.skPam.clientMsg.clientFdIndex++] = accept(appCfgPrm.skPam.srvFd,(struct sockaddr* )&(appCfgPrm.skPam.cltAddr),&appCfgPrm.skPam.skaddrLen);
		if(appCfgPrm.skPam.clientMsg.clientFd[appCfgPrm.skPam.clientMsg.clientFdIndex -1] < 0){
			printf("======get clientfd fail==========================\n");
		}else{
			dump_client_message();
		}
	}
}
char create_socket_server()
{
	int ret = -1;
	init_socket_param();
	
	ret = start_up();
	if(ret < 0){
		printf("\r\n===========start up fail=================\r\n");
	}
	xTaskCreate(receive_data,"socketServerData",1024 * 4,0,5,0);
	return ret;
}

inline void init_config_param()
{
	memcpy(configParam.alm_param,sdkParam.alm_param,ALARM_NUM * sizeof(alarm_param));
	configParam.brightness = sdkParam.brightness;
	configParam.city = sdkParam.wth_parma.cityId;
	configParam.nowtime = sdkParam.nt_param.isOpen;
	configParam.temp = sdkParam.temperature;
	
	
}
void config_param()
{
	printf("\r\n=========starting config param============\r\n");
	appCfgPrm.skPam.skaddrLen = sizeof(struct sockaddr_in);
	appCfgPrm.skPam.srvPort = 80;
	create_socket_server();
	config_load();
	config_get(&sdkParam);
	init_config_param();
	while(1) {
		app_accept();
	}
}
#endif
#endif
