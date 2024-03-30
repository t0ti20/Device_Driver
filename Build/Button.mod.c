#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x30a38ba5, "module_layout" },
	{ 0x4ccc0c5, "gpiod_direction_input" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xa7ce533d, "device_create" },
	{ 0x183c9e7f, "__class_create" },
	{ 0xdad1877a, "cdev_add" },
	{ 0x69a723ec, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xcfad5bb0, "cdev_del" },
	{ 0xb00c7c36, "class_destroy" },
	{ 0xcd15882b, "device_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xa519dd4d, "gpiod_get_raw_value" },
	{ 0x72ab4f7b, "gpio_to_desc" },
	{ 0x92997ed8, "_printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3C60BF002028F81D626A63E");
