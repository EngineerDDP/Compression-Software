namespace HuffmanCodec
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.label1 = new System.Windows.Forms.Label();
			this.OutputFile = new System.Windows.Forms.TextBox();
			this.Save = new System.Windows.Forms.Button();
			this.Compress = new System.Windows.Forms.Button();
			this.StatusInfo = new System.Windows.Forms.RichTextBox();
			this.SaveCompressedFile = new System.Windows.Forms.SaveFileDialog();
			this.InputFile = new System.Windows.Forms.TextBox();
			this.OpenTargetFile = new System.Windows.Forms.OpenFileDialog();
			this.BlockSize = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(107, 25);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(65, 12);
			this.label1.TabIndex = 8;
			this.label1.Text = "压缩文件名";
			// 
			// OutputFile
			// 
			this.OutputFile.Location = new System.Drawing.Point(109, 40);
			this.OutputFile.Name = "OutputFile";
			this.OutputFile.Size = new System.Drawing.Size(224, 21);
			this.OutputFile.TabIndex = 3;
			// 
			// Save
			// 
			this.Save.Location = new System.Drawing.Point(258, 67);
			this.Save.Name = "Save";
			this.Save.Size = new System.Drawing.Size(75, 23);
			this.Save.TabIndex = 4;
			this.Save.Text = "浏览";
			this.Save.UseVisualStyleBackColor = true;
			this.Save.Click += new System.EventHandler(this.Save_Click);
			// 
			// Compress
			// 
			this.Compress.Enabled = false;
			this.Compress.Location = new System.Drawing.Point(258, 326);
			this.Compress.Name = "Compress";
			this.Compress.Size = new System.Drawing.Size(75, 23);
			this.Compress.TabIndex = 5;
			this.Compress.Text = "压缩";
			this.Compress.UseVisualStyleBackColor = true;
			this.Compress.Click += new System.EventHandler(this.Compress_Click);
			// 
			// StatusInfo
			// 
			this.StatusInfo.Cursor = System.Windows.Forms.Cursors.Arrow;
			this.StatusInfo.Location = new System.Drawing.Point(29, 96);
			this.StatusInfo.Name = "StatusInfo";
			this.StatusInfo.ReadOnly = true;
			this.StatusInfo.Size = new System.Drawing.Size(304, 224);
			this.StatusInfo.TabIndex = 4;
			this.StatusInfo.TabStop = false;
			this.StatusInfo.Text = "";
			// 
			// SaveCompressedFile
			// 
			this.SaveCompressedFile.DefaultExt = "hed";
			this.SaveCompressedFile.Filter = "压缩文件|*.hed";
			this.SaveCompressedFile.FileOk += new System.ComponentModel.CancelEventHandler(this.SaveCompressedFile_FileOk);
			// 
			// InputFile
			// 
			this.InputFile.Location = new System.Drawing.Point(29, 326);
			this.InputFile.Name = "InputFile";
			this.InputFile.Size = new System.Drawing.Size(224, 21);
			this.InputFile.TabIndex = 1;
			this.InputFile.Visible = false;
			// 
			// OpenTargetFile
			// 
			this.OpenTargetFile.Filter = "文本文件|*.txt";
			this.OpenTargetFile.FileOk += new System.ComponentModel.CancelEventHandler(this.OpenTargetFile_FileOk);
			// 
			// BlockSize
			// 
			this.BlockSize.FormattingEnabled = true;
			this.BlockSize.Items.AddRange(new object[] {
            "1",
            "2"});
			this.BlockSize.Location = new System.Drawing.Point(29, 40);
			this.BlockSize.Name = "BlockSize";
			this.BlockSize.Size = new System.Drawing.Size(63, 20);
			this.BlockSize.TabIndex = 9;
			this.BlockSize.Text = "2";
			this.BlockSize.SelectedIndexChanged += new System.EventHandler(this.BlockSize_SelectedIndexChanged);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(27, 25);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(53, 12);
			this.label2.TabIndex = 10;
			this.label2.Text = "字典大小";
			// 
			// MainForm
			// 
			this.AcceptButton = this.Compress;
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(366, 364);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.BlockSize);
			this.Controls.Add(this.InputFile);
			this.Controls.Add(this.StatusInfo);
			this.Controls.Add(this.Compress);
			this.Controls.Add(this.Save);
			this.Controls.Add(this.OutputFile);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.Text = "压缩文件名和参数";
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.File_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.File_DragEnter);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox OutputFile;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button Compress;
        private System.Windows.Forms.RichTextBox StatusInfo;
        private System.Windows.Forms.SaveFileDialog SaveCompressedFile;
        private System.Windows.Forms.TextBox InputFile;
        private System.Windows.Forms.OpenFileDialog OpenTargetFile;
		private System.Windows.Forms.ComboBox BlockSize;
		private System.Windows.Forms.Label label2;
	}
}

