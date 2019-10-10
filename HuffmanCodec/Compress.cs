using System;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;

namespace HuffmanCodec
{
	public partial class MainForm : Form
    {
        /// <summary>
        /// 压缩文件的后缀名
        /// </summary>
        const string Extension = ".hed";
        public MainForm()
        {
            InitializeComponent();
        }
        /// <summary>
        /// 带参调用
        /// </summary>
        /// <param name="filename">文件名参数</param>
        public MainForm(String filename)
        {
            InitializeComponent();
            SetFileName(filename);
        }

        /// <summary>
        /// 设置可压缩的文件名
        /// </summary>
        /// <param name="filename"></param>
        private void SetFileName(string filename)
        {
            InputFile.Text = filename;
            OutputFile.Text = Path.GetDirectoryName(filename) + '\\' + Path.GetFileNameWithoutExtension(filename) + Extension;
            SaveCompressedFile.FileName = OutputFile.Text;
			Compress.Enabled = true;
        }

        #region 拖拽文件
        /// <summary>
        /// 拖拽文件进入
        /// </summary>
        private void File_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] temp = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (temp.Length == 1 && Path.GetExtension(temp[0]) == ".txt")
                    e.Effect = DragDropEffects.Link;
            }
        }

        /// <summary>
        /// 拖拽文件
        /// </summary>
        private void File_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] temp = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (temp.Length == 1 && Path.GetExtension(temp[0]) == ".txt")
                {
                    SetFileName(temp[0]);
                }
            }
        }
        #endregion

        /// <summary>
        /// 点击打开文件
        /// </summary>
        private void Open_Click(object sender, EventArgs e)
        {
            OpenTargetFile.ShowDialog(this);
        }

        /// <summary>
        /// 点击保存压缩文件
        /// </summary>
        private void Save_Click(object sender, EventArgs e)
        {
            SaveCompressedFile.ShowDialog(this);
        }

        /// <summary>
        /// 点击确定打开
        /// </summary>
        private void OpenTargetFile_FileOk(object sender, CancelEventArgs e)
        {
            SetFileName(OpenTargetFile.FileName);
        }

        /// <summary>
        /// 点击确定保存
        /// </summary>
        private void SaveCompressedFile_FileOk(object sender, CancelEventArgs e)
        {
            OutputFile.Text = SaveCompressedFile.FileName;
        }

        /// <summary>
        /// 点击压缩
        /// </summary>
        private void Compress_Click(object sender, EventArgs e)
        {
			FileStream fs = File.OpenRead(InputFile.Text);
			try
			{
				StatusInfo.Clear();
				Compress.Enabled = false;

				StatusText = "分析文件......";

				StatusText = "加载哈夫曼编码编码......";

				Codex.Analyzer a = new Codex.Analyzer(fs);
				Codex.HuffmanCodex h = new Codex.HuffmanCodex(a);

				StatusText = "写入哈夫曼编码......";

				fs.Position = 0;
				Codex.HuffmanEncoder en = new Codex.HuffmanEncoder(h, fs);
				FileStream target = File.OpenWrite(OutputFile.Text);
				target.SetLength(0);
				target.WriteByte((Byte)Codex.Block.BlockSize);      //文件第一位写块大小
				en.WriteCodexToStream(target);

				StatusText = "写入压缩文件......";

				var buffer = en.ReadToEnd();
				target.Write(buffer, 0, buffer.Length);

				StatusText = "写入完成.";
				StatusText = "压缩率:" + (100.0 * target.Length / fs.Length) + '%';
				StatusText = "平均编码长度:" + Codex.Debug.AverageCodeLength(a, h) + "位";
				StatusText = "字典大小:" + Codex.Block.BlockSize + "字节";
				StatusText = "压缩文件头长度:" + h.Length + "字节";
				target.Close();
			}
			catch(DataMisalignedException)
			{
				MessageBox.Show("文件长度不能够均分到每个块中", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			fs.Close();
			Compress.Enabled = true;
        }

		private string StatusText
		{
			set
			{
				StatusInfo.Text += value + '\n';
			}
		}

		private void BlockSize_SelectedIndexChanged(object sender, EventArgs e)
		{
			Codex.Block.BlockSize = Int32.Parse(BlockSize.SelectedItem.ToString());
		}
	}
}
