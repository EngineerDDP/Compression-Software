namespace HuffmanCodec
{
    partial class Decompress
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			this.DecompProgress = new System.Windows.Forms.ProgressBar();
			this.Cancel = new System.Windows.Forms.Button();
			this.Label = new System.Windows.Forms.Label();
			this.ProgressRefrash = new System.Windows.Forms.Timer(this.components);
			this.Filename = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// DecompProgress
			// 
			this.DecompProgress.Location = new System.Drawing.Point(13, 80);
			this.DecompProgress.Name = "DecompProgress";
			this.DecompProgress.Size = new System.Drawing.Size(304, 15);
			this.DecompProgress.Step = 1;
			this.DecompProgress.TabIndex = 0;
			// 
			// Cancel
			// 
			this.Cancel.Location = new System.Drawing.Point(242, 126);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(75, 23);
			this.Cancel.TabIndex = 1;
			this.Cancel.Text = "取消";
			this.Cancel.UseVisualStyleBackColor = true;
			// 
			// Label
			// 
			this.Label.AutoSize = true;
			this.Label.Location = new System.Drawing.Point(12, 15);
			this.Label.Name = "Label";
			this.Label.Size = new System.Drawing.Size(47, 12);
			this.Label.TabIndex = 2;
			this.Label.Text = "解压到:";
			// 
			// ProgressRefrash
			// 
			this.ProgressRefrash.Enabled = true;
			this.ProgressRefrash.Interval = 10;
			this.ProgressRefrash.Tick += new System.EventHandler(this.ProgressRefrash_Tick);
			// 
			// Filename
			// 
			this.Filename.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Filename.Location = new System.Drawing.Point(14, 30);
			this.Filename.Multiline = true;
			this.Filename.Name = "Filename";
			this.Filename.ReadOnly = true;
			this.Filename.Size = new System.Drawing.Size(303, 44);
			this.Filename.TabIndex = 3;
			// 
			// Decompress
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(329, 161);
			this.Controls.Add(this.Filename);
			this.Controls.Add(this.Label);
			this.Controls.Add(this.Cancel);
			this.Controls.Add(this.DecompProgress);
			this.Cursor = System.Windows.Forms.Cursors.Default;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "Decompress";
			this.Text = "解压缩";
			this.Load += new System.EventHandler(this.Decompress_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ProgressBar DecompProgress;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.Label Label;
		private System.Windows.Forms.Timer ProgressRefrash;
		private System.Windows.Forms.TextBox Filename;
	}
}