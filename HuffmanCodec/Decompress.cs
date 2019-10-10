using System;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace HuffmanCodec
{
	public partial class Decompress : Form
    {
		private Thread DecodeProcess;
		private int progress;
		private string savefile;
		/// <summary>
		/// 接收文件参数并启动程序
		/// </summary>
		/// <param name="filename"></param>
        public Decompress(String filename)
        {
            InitializeComponent();
            Filename.Text = filename;
			progress = 0;
        }
		/// <summary>
		/// 加载文件
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Decompress_Load(object sender, EventArgs e)
		{
			if (Path.GetExtension(Filename.Text) == ".hed")
			{
				DecodeProcess = new Thread(DeCodeFile);
				DecodeProcess.Start();
				DecodeProcess.Name = "解码子线程";
				Cancel.Enabled = true;
			}
			else
			{
				this.Close();
			}
		}
		/// <summary>
		/// 解码
		/// </summary>
		/// <param name="o"></param>
		private void DeCodeFile(object o)
		{
			FileStream fs = File.OpenRead(Filename.Text);
			try
			{
				Codex.Block.BlockSize = fs.ReadByte();      //文件第一位读块大小
				savefile = new string(Filename.Text.Take(Filename.Text.Length - 4).ToArray());
				savefile += ".txt";
				progress += 10;
				FileStream fo = File.OpenWrite(savefile);
				progress += 10;
				Codex.HuffmanDecoder d = new Codex.HuffmanDecoder(fs);
				progress += 10;
				Byte[] buffer = d.ReadToEnd();
				progress += 30;
				fo.Write(buffer, 0, buffer.Length);
				progress += 30;
				fo.Flush();
				fo.Close();
			}
			catch(DataMisalignedException)
			{
				MessageBox.Show("压缩文件头已损坏", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			fs.Close();
			progress = 100;
		}
		/// <summary>
		/// 刷新进度条
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void ProgressRefrash_Tick(object sender, EventArgs e)
		{
			DecompProgress.Value = progress;
			if(savefile != null)
				Filename.Text = savefile;
			if (progress == 100)
			{
				Cancel.Enabled = false;
				ProgressRefrash.Enabled = false;
				MessageBox.Show("解压完成");
				this.Close();
			}
		}
	}
}
