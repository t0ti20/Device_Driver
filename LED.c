/* Registeration Files */
#include <linux/fs.h>
/* Adding Parametars */
#include <linux/moduleparam.h>
/* Module Structure */
#include <linux/module.h>
/* Generate Mojor Number Automatically */
#include <linux/cdev.h>
/* Initialize Module */
#include <linux/init.h>
/* GPIO Module */
#include <linux/gpio.h>

/* Define Controlled LED Pin */
#define LED_PIN 18
/* Define Buffer Size */
#define BUFFER_SIZE 255
/* Driver Numbers */
#define BASE_NUMBER 0
#define DEVICES_COUNT 1

/* Initialize Reading Buffer */
static unsigned char File_Buffer[BUFFER_SIZE]={};

/* Write Call Function */
ssize_t Write_Device_File(struct file *Device_File,const char *User_Buffer,size_t Write_Bytes_Count,loff_t *Offset)
{
    ssize_t Return=0;
    printk("Writing Data To File (Bytes=%ld | Offset=%lld)\n",Write_Bytes_Count,*Offset);
    /* Check For Write Count Size */
    if((Write_Bytes_Count+*Offset)>BUFFER_SIZE){Write_Bytes_Count=BUFFER_SIZE-*Offset;}
    if(Write_Bytes_Count!=0)
    {
        /* Copy From Physical Address To Logical Address */
        if(copy_from_user(&File_Buffer[*Offset],User_Buffer,Write_Bytes_Count)==0)
        {
            Return=Write_Bytes_Count;
            *Offset=Write_Bytes_Count;
            printk("Data has Written Successfully\n");
            /* Check For Character */
            gpio_set_value(LED_PIN,File_Buffer[0]=='1'?1:0);
        }
        else
        {
            Return=-1;
            printk("Can't Write All Data\n");
        }
    }
    else
    {
        printk("No Space To Write\n");
        File_Buffer[BUFFER_SIZE-1]='\n';
        Return=-1;
    }
    return Return;
}

/* Read Call Function */
ssize_t Read_Device_File(struct file *Device_File,char __user *User_Buffer,size_t Read_Bytes_Count,loff_t *Offset)
{
    ssize_t Return=0;
    printk("Reading Data From File (Bytes=%ld | Offset=%lld)\n",Read_Bytes_Count,*Offset);
    /*Update Buffer*/
    File_Buffer[0]=gpio_get_value(LED_PIN)+'0';
    File_Buffer[1]='\n';
    /* Check For Read Count Size */
    if((Read_Bytes_Count+*Offset)>BUFFER_SIZE){Read_Bytes_Count=BUFFER_SIZE-*Offset;}
    /* Copy From Physical Address To Logical Address */
    if(copy_to_user(User_Buffer,&File_Buffer[*Offset],Read_Bytes_Count)==0)
    {
        Return=Read_Bytes_Count;
        printk("Data has Readded Successfully\n");
    }
    else
    {
        Return=-1;
        printk("Can't Read Whole File\n");
    }
    *Offset=Read_Bytes_Count;
    return Return;
}
/* Open Call Function */
static int Open_Device_File(struct inode *Device_Inode,struct file *Object)
{
    printk("Driver File Opened !\n");
    return 0;
}

/* CLose Call Function */
static int CLose_Device_File(struct inode *Device_Inode,struct file *Object)
{
    printk("Driver File CLosed !\n");
    return 0;
}

/* File Call Operation */
struct file_operations Led_File =
{
    .owner=THIS_MODULE,
    .open=Open_Device_File,
    .release=CLose_Device_File,
    .read=Read_Device_File,
    .write=Write_Device_File
};

/* To Store Major And Minor Number */
dev_t Device_Number;

/* Character Module File Handlar */
struct cdev Module_File;
/* Character Device Class Handlar */
struct class *Device_Class;
/* Character Device Class Handlar */
struct device *Device_File;

/* Delete Any Initialized Element Before Exit */
void Delete_Registered_Elements(void)
{
    /* Reset Value To Default */
    gpio_set_value(LED_PIN,0);
    /* Free Requested GPIO */
    gpio_free(LED_PIN);
    /* Delete Device File */
    device_destroy(Device_Class,Device_Number);
    /* Delete Class File */
    class_destroy(Device_Class);
    /* Delete Module */
    cdev_del(&Module_File);
    /* Delete Registered Char Device */
    unregister_chrdev_region(Device_Number,DEVICES_COUNT);
}

/* Initialization Function For Diver */
static int __init LED_Initialization(void)
{
    int Return_Value=0;
    /* Print Welcome Message */
    printk("Starting LED Driver\n");
    /* Allocate Char Device */
    if(alloc_chrdev_region(&Device_Number,BASE_NUMBER,DEVICES_COUNT,"Control_LED")==0)
    {
        printk("Done Registering Char Device (Major=%d | Minor=%d | Function=%s)\n",MAJOR(Device_Number),MINOR(Device_Number),__FUNCTION__);
        /* Initialization Character Device Module */
        cdev_init(&Module_File,&Led_File);
        /* Create Character Device Module */
        if(cdev_add(&Module_File,Device_Number,DEVICES_COUNT)==0)
        {
            printk("Done Createing Char Module (Major=%d | Minor=%d | Function=%s)\n",MAJOR(Device_Number),MINOR(Device_Number),__FUNCTION__);
            /*Create Class In Order To Create Device File*/
            Device_Class=class_create(THIS_MODULE,"Control_LED");
            if(Device_Class!=NULL)
            {
                /* Create File From Class */
                Device_File=device_create(Device_Class,NULL,Device_Number,NULL,"Control_LED");
                if(Device_File!=NULL)
                {
                    printk("Done Createing Device File (Major=%d | Minor=%d | Function=%s)\n",MAJOR(Device_Number),MINOR(Device_Number),__FUNCTION__);
                }
                else
                {
                    printk("Can't Create Device File !\n");
                    Return_Value=-1;
                }
            }
            else
            {
                printk("Can't Create Class File !\n");
                Return_Value=-1;
            }
        }
        else
        {
            printk("Can't Create Char Module File !\n");
            Return_Value=-1;
        }
    }
    else 
    {
        printk("Can't Allocate Memory For Char Device !\n");
        Return_Value=-1;
    }
    /* Initialize GPIO */
    if((gpio_request(LED_PIN,"LED_Pin")==0)&&(Return_Value==0))
    {
        /* Set Pin To Be Output */
        if(gpio_direction_output(LED_PIN,0)==0)
        {
            printk("GPIO Pin Initialized Successfully\n");
        }
        else
        {
            printk("Can't Set GPIO Pin As Output !\n");
            Return_Value=-1;
        }
    }
    else
    {
        printk("Can't Request GPIO Pin !\n");
        Return_Value=-1;
    }
    if(Return_Value!=0){Delete_Registered_Elements();}
    return Return_Value;
}

/* Exit Function For Diver */
static void __exit LED_Exit(void)
{
    /* Delete Registered Devices */
    Delete_Registered_Elements();
    /* Print Exit Message */
    printk("Ending LED Driver\n");
}

/*Initialize Module*/
module_init(LED_Initialization);
/*Delete Module*/
module_exit(LED_Exit);

/* Set Meta Information  */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khaled El-Sayed");
MODULE_DESCRIPTION("LED Driver for controling LED");

