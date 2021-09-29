# 键盘监控

## 打开设备
打开键盘输入设备`/dev/input/event3`
```C++
#include <fcntl.h>
#include <linux/input.h>

	this->kb = open("/dev/input/event3", O_RDWR);
    if (this->kb == -1) {
        perror("open input ERROR");
    }

	// 这里采用QT的QSocketNotifier类来触发事件
    kbNotifer = new QSocketNotifier(this->kb, QSocketNotifier::Read);
    QObject::connect(this->kbNotifer, &QSocketNotifier::activated, this,
                     &KBListener::onKeyPressed);
```

这里采用的Qt的QSocketNotifier类来监控其可读性，可读（用户有输入）时候，即触发QSocketNotifier::activated信号，从而关联我们的处理槽函数即可。

也可以使用其他的事件循环来实现。比如linux的select方法等。



## 读取设备内容并处理

```C++
#include <QSocketNotifier>

struct input_event kbEvent;

void KBListener::onKeyPressed(qintptr fd) {
    if (read(fd, &kbEvent, sizeof(kbEvent)) == sizeof(kbEvent)) {
        if (kbEvent.type == EV_KEY) {
            if (kbEvent.value == 1) {
                if (this->onScreen && kbEvent.code == KEY_ESC) {
                    emit KEY_Esc();
                }
                if (this->onScreen && kbEvent.code == KEY_UP) {
                    emit KEY_Up();
                }
                if (this->onScreen && kbEvent.code == KEY_DOWN) {
                    emit KEY_Down();
                }
                if (kbEvent.code == KEY_P) {
                    this->kbP = true;
                }
                if (kbEvent.code == KEY_LEFTMETA ||
                        kbEvent.code == KEY_RIGHTMETA) {
                    this->kbCtrl = true;
                }
            } else {
                if (kbEvent.code == KEY_P) {
                    this->kbP = false;
                }
                if (kbEvent.code == KEY_LEFTMETA ||
                        kbEvent.code == KEY_RIGHTMETA) {
                    this->kbCtrl = false;
                }
            }
        }
        if (this->kbCtrl && this->kbP) {
            emit KEY_Show();
        }
    }
}
```

读取时，是`linux/input.h`中的`struct input_event`结构体类型，其基本结构如下
```C++
struct input_event {
#if (__BITS_PER_LONG != 32 || !defined(__USE_TIME_BITS64)) && !defined(__KERNEL__)
	struct timeval time;
#define input_event_sec time.tv_sec
#define input_event_usec time.tv_usec
#else
	__kernel_ulong_t __sec;
#if defined(__sparc__) && defined(__arch64__)
	unsigned int __usec;
	unsigned int __pad;
#else
	__kernel_ulong_t __usec;
#endif
#define input_event_sec  __sec
#define input_event_usec __usec
#endif
	__u16 type;
	__u16 code;
	__s32 value;
};
```