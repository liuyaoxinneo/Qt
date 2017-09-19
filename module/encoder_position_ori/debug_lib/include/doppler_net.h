#ifndef DOPPLER_NET_H
#define DOPPLER_NET_H

#include "doppler_net_global.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
#include <QObject>

#include "doppler_command.h"
#include "doppler_structure.h"

static const int TCP_PORT = 8000;
static const int UDP_PORT = 6000;

class DOPPLER_NETSHARED_EXPORT Doppler_net: public QObject
{
    Q_OBJECT
public:
    Doppler_net(QObject *parent = Q_NULLPTR);
    ~Doppler_net();
    bool tcp_connect(QString ip);
    bool udp_connect();

    /*
     *发命令操作仪器
     */
    bool reset_all();
    bool reset_encoder();
    bool set_current_group(int groupId);
    bool add_group(int groupId);
    bool delete_group(int groupId);
    bool set_encoder_type(ENCODER_TYPE type);
    struct config_scan scan_info();
    bool set_scan_info(struct config_scan info);
    struct config_focallaw focallaw_info(int group);
    bool set_focallaw_info(struct config_focallaw info);

    CONFIG_PROBE_INFO probe_info();
    bool set_probe_info(CONFIG_PROBE_INFO info);

    CONFIG_WEDGE_INFO wedge_info();
    bool set_wedge_info(CONFIG_WEDGE_INFO info);

    CONFIG_PART_INFO part_info();
    bool set_part_info(CONFIG_PART_INFO info);

    CONFIG_UT_GENERAL general_info();
    bool set_general_info(CONFIG_UT_GENERAL info);

    CONFIG_UT_PULSER pulser_info();
    bool set_pulser_info(CONFIG_UT_PULSER info);

    CONFIG_UT_RECEIVER receiver_info();
    bool set_receiver_info(CONFIG_UT_RECEIVER info);

    CONFIG_UT_ADVANCED advance_info();
    bool set_advance_info(CONFIG_UT_ADVANCED info);

    CONFIG_GATE_INFO gate_info(int gateId);
    bool set_gate_info(CONFIG_GATE_INFO info);

    CONFIG_DISPLAY_INFO display_info();
    bool set_display_info(CONFIG_DISPLAY_INFO info);

    CONFIG_GROUP_BEAMDELAY beam_delay();
    bool set_beam_delay(CONFIG_GROUP_BEAMDELAY info);

    CONFIG_GROUP_GAINOFFSET gate_offset();
    bool set_gate_offset(CONFIG_GROUP_GAINOFFSET info);

    CONFIG_SIZING_CURVE sizing_curve();
    bool set_sizing_curve(CONFIG_SIZING_CURVE info);

    bool enable_data_transfer(bool flag);/*启动采样数据传输*/
    int group_qty();/*组的总数*/
    int tatal_data_size();
    int group_data_size();
    int group_data_offset(int groupId);
    int group_beam_offset(int groupId);
    int group_beam_length(int groupId);

    int gain();/*获取到的值是仪器 显示值*10.*/
    bool set_gain(int gainVal);/*设置值也要 *10 如：设置增益为20，gainVal的值要写200.*/

    /*@function  start_val()
     *@return:
     *如果聚焦类型是 真实深度
     *  如果单位是mm：返回值=(((int)(仪器显示起始点的值*2000000.0/(cos(当前角度)*声速(仪器上显示的声速值)))+5)/10)*10;
     *  如果单位是英寸：返回值=(((int)(仪器显示起始点的值*2000000.0/(cos(当前角度)*0.03937*声速(仪器上显示的声速值)))+5)/10)*10;
     *如果聚焦类型是 声程
     *  如果单位是mm：返回值=(((int)(仪器显示起始点的值*2000000.0/声速(仪器上显示的声速值))+5)/10)*10;
     *  如果单位是英寸：返回值=(((int)(仪器显示起始点的值*2000000.0/(0.03937*声速(仪器上显示的声速值)))+5)/10)*10;
     *
     *如果聚焦类型是其他
     *  返回值 = (((int)(仪器显示起始点的值*1000.0)+5)/10)*10;
     */
    int start_val();
    bool set_start_val(int val);

    /*@function  range()
     *@return:
     *如果聚焦类型是 真实深度
     *  如果单位是mm：返回值=((int)(仪器显示起始点的值*2000000.0/(cos(当前角度)*声速(仪器上显示的声速值))) + 点数*10/2) *(点数*10);
     *  如果单位是英寸：返回值=((int)(仪器显示起始点的值*2000000.0/(cos(当前角度)*0.03937*声速(仪器上显示的声速值))) + 点数*10/2) *(点数*10);
     *如果聚焦类型是 声程
     *  如果单位是mm：返回值=((int)(仪器显示起始点的值*2000000.0/声速(仪器上显示的声速值)) + 点数*10/2) *(点数*10);
     *  如果单位是英寸：返回值=((int)(仪器显示起始点的值*2000000.0/(0.03937*声速(仪器上显示的声速值))) + 点数*10/2) *(点数*10);
     *
     *如果聚焦类型是其他
     * 返回值 = ((int)(仪器显示起始点的值*1000.0) + 点数*10/2) *(点数*10);
     */
    int range();
    bool set_rangs(int val);

    int cur_group_point_qty();
    bool set_cur_group_point_qty(int val);

    int velocity();
    bool set_velocity(int val);

    int wedge_delay();
    bool set_wedge_delay(int val);

    int prf();
    bool set_prf(int val);

private:
    bool send_data_to_server(ulong command,ulong len,void *data);
    bool get_data_from_server(void*recvdata,size_t size);
signals:
    void capture_event(const QByteArray &data);
protected slots:
    void do_readyread();

private:
    QString m_ip;
    QTcpSocket m_tcpSocket;
    QUdpSocket m_udpSocket;
};

#endif // DOPPLER_NET_H
