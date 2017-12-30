#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/io.h>

MODULE_AUTHOR("TTT");//作者
MODULE_DESCRIPTION("driver for LED control");//モジュールの説明
MODULE_LICENSE("GPL");//ライセンス（基本的にGPL）
MODULE_VERSION("0.1");//バーション

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;
	
	if(c=='0')
		gpio_base[10] = 1<<25;//GPIO Pin Output clear 0
	else if(c=='1')
		gpio_base[7] = 1 << 25;//GPIO Pin Output clear 1

	printk(KERN_INFO "receive %c\n",c);
	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};

//モジュール初期設定
static int __init init_mod(void)
{
	int retval;

	gpio_base = ioremap_nocache(0x3f200000, 0xA0);

	const u32 led = 25;//ledの位置指定
	const u32 index = led/10; //25ピンの住所を求める
	//0(0~9):1111~1111
	//1(10~19):1111~1111
	//2(20~29):1111~1111←ここ
	const u32 shift = (led%10) * 3; //25ピンの番地をもとめる
	//1111111111111"111"1111111111111
	const u32 mask = ~(0x7 << shift);
	//25ピンのところまで0にする 
	//1111111111111"000"1111111111111
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
	//25ピンはledを光らせるのでoutput(001)にする
	//1111111111111"001"1111111111111
	
	retval = alloc_chrdev_region(&dev,0,1,"myled");
	//デバイス番号の取得
	//引数：dev(番号の入れ物）のアドレス，0番から1個マイナー番号を取得)
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;	
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
	//カーネルモジュールの着確認
	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
	return 0;
}

//モジュールの解放（終了時の処理）
static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls,dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major%d\n",__FILE__,MAJOR(dev));
	//カーネルモジュールの脱確認
}

module_init(init_mod);
module_exit(cleanup_mod);
