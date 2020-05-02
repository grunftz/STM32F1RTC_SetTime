using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Globalization;

namespace STM32F10x__Date_Time_Loading
{
    public partial class Form1 : Form
    {
        //-------------------VARßs-------------------------
        string[] comPort = SerialPort.GetPortNames();
        string data;
        
        //-------------------------------------------------
        public Form1()
        {
            InitializeComponent();
            //-----------GET PORT NAMES-----------------
            foreach (var portIme in comPort)
            {
                comboBox1.Items.Add(portIme);
            }
            comboBox1.SelectedIndex =0;
            //-------------------------------------------
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            toolStripStatusLabel1.Text = "DISCONNECTED!";

        }

        private void btnCooPort_Click(object sender, EventArgs e)
        {

            if (!serialPort1.IsOpen)
            {
                try
                {
                    serialPort1.Open();
                    toolStripStatusLabel1.Text = comboBox1.SelectedItem.ToString()+ " CONNECTED!";
                    btnCooPort.Text = "Usart Close!";
                    comboBox1.Enabled = false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
            }
            else
            {
                serialPort1.Close();
                btnCooPort.Text = "Usart Open!";
                toolStripStatusLabel1.Text = " DISCONNECTED!";
                comboBox1.Enabled = true;

            }
        }
              private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.PortName = comboBox1.SelectedItem.ToString();

        }

 

        private void btn_Click(object sender, EventArgs e)
        {
            
                var time = DateTime.Now;
                lblHours.Text = time.Hour.ToString();
                if (time.Minute > 9)
                {
                    lblMinutes.Text = time.Minute.ToString();
                }
                else
                {
                    lblMinutes.Text = "0" + time.Minute.ToString();
                }
                
                if (time.Second > 9)
                {
                    lblSec.Text = time.Second.ToString();
                }
                else 
                {
                    lblSec.Text = "0"+time.Second.ToString();
                }
                Console.WriteLine(DateTime.Now.ToString());
            //conversion form time to RTC_count
            int h = time.Hour * 3600;
            int m = ((time.Minute * 60) + h) % 3600;
            int midlle = h + m;
            int s = (((time.Second) - (midlle)) % 60);

            int count_RTC = h + m + (s+60);

            Console.WriteLine(h);

            Console.WriteLine(m);
            Console.WriteLine(s+60);
         
            Console.WriteLine(count_RTC);
            if (!serialPort1.IsOpen)
            {
                MessageBox.Show("Com Port Closed!");
            }
            else
            {

                serialPort1.Write(count_RTC.ToString().Length + count_RTC.ToString());
            }
               
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data = serialPort1.ReadLine();
            this.Invoke(new EventHandler(nekafunk));

        }


        private void nekafunk(object sender, EventArgs e)
        {
            textBox1.Text = data;
        }



    }

    }