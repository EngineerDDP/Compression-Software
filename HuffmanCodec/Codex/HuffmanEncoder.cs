using System;
using System.Collections.Generic;
using System.IO;

namespace HuffmanCodec.Codex
{
	struct BitSet
	{
		public Byte buffer;
		public Byte length;
		public const Int32 SizeofThis = 8;

		public BitSet(byte buffer, byte length)
		{
			this.buffer = buffer;
			this.length = length;
		}
		/// <summary>
		/// 在结尾添加一位
		/// </summary>
		/// <param name="bit"></param>
		public void AddBitBack(int bit)
		{
			if (length == SizeofThis)
				throw new ArgumentOutOfRangeException("4位字长不足以容纳指定的bit值");
			else
			{
				buffer = (Byte)((buffer << 1) | bit);
				++length;
			}
		}
		/// <summary>
		/// 转String，调试用
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			return Convert.ToString(buffer, 2).PadLeft(length, '0');
		}
	}
    class HuffmanEncoder
    {
        HuffmanCodex codex;
        Stream source;
		/// <summary>
		/// 通过已生成的哈夫曼编码和要编码的数据流创建编码器
		/// </summary>
		/// <param name="codex">哈夫曼编码表</param>
		/// <param name="s">要编码的字符流</param>
        public HuffmanEncoder(HuffmanCodex codex, Stream s)
        {
            this.codex = codex;
            this.source = s;
        }
        /// <summary>
        /// 读指定数目字符编码得到的字节,该方法过于冗余已淘汰,使用ReadToEnd来读取所有字符并编码而不是该方法
        /// </summary>
        /// <param name="buffer">缓冲区</param>
        /// <param name="count">字符数目</param>
        /// <param name="offset">开始写入的位置</param>
        private int Read(Byte[] buffer, int offset, int count)
        {
			throw new NotImplementedException();
        }
		/// <summary>
		/// 一直读到文件流结尾并输出对应的字节串
		/// </summary>
		/// <returns></returns>
		public Byte[] ReadToEnd()
		{
			Codec value = new Codec(0, 0);
			Block c;
			int i;
			int length;
			BitSet temp = new BitSet(0, 0);
			List<Byte> result = new List<byte>();
			//一直读取直到文件结尾
			while(source.Position != source.Length)
			{
				c = new Block(source);
				value = codex.Codex[c];
				//将哈夫曼编码逐位写入结果
				for (i = value.length - 1; i >= 0; --i) 
				{
					temp.AddBitBack((value.value >> i) & 1);
					if (temp.length == BitSet.SizeofThis)
					{
						result.Add(temp.buffer);
						temp = new BitSet(0, 0);
					}
				}
			}
			length = result.Count * BitSet.SizeofThis;
			if (temp.length != BitSet.SizeofThis)
			{
				temp.buffer = (Byte)(temp.buffer << (BitSet.SizeofThis - temp.length));
				result.Add(temp.buffer);
				length += temp.length;
			}
			result.InsertRange(0, Codec.WriteInt(length, 4));
			return result.ToArray();
		}
        /// <summary>
        /// 向流中写入哈夫曼编码
        /// </summary>
        /// <param name="s"></param>
        public void WriteCodexToStream(Stream s)
        {
			Byte[] temp = Codec.WriteInt(codex.Length, 4);
			s.Write(temp, 0, temp.Length);
            foreach(Block i in codex.Codex.Keys)
            {
				s.Write(i.ToBytes(), 0, Block.BlockSize);
                s.Write(codex.Codex[i].ToBytes(), 0, Codec.SizeofThis);
            }
        }
		/// <summary>
		/// 获取编码器中哈夫曼编码的字符串形式
		/// </summary>
		/// <returns></returns>
		public string GetCodex()
		{
			string t = "";
			foreach (Block i in codex.Codex.Keys)
			{
				t += i + " : " + codex.Codex[i] + "\r\n";
			}
			return t;
		}
	}
}
