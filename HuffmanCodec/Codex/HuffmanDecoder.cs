using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace HuffmanCodec.Codex
{
    class HuffmanDecoder
    {
		HuffmanCodex codex;
		FileStream source;
		/// <summary>
		/// 通过一个文件流创建哈夫曼解码器
		/// </summary>
		/// <param name="f"></param>
		public HuffmanDecoder(FileStream f)
		{
			source = f;
			Int32 i;
			byte[] buffer = new byte[4];
			f.Read(buffer, 0, 4);		//读取文件头标记,获取编码表的长度
			i = Codec.ReadInt(buffer, 4);
			buffer = new byte[i];		//读取文件头中编码表
			f.Read(buffer, 0, i);
			codex = new HuffmanCodex(buffer);	//反序列化哈夫曼编码
		}
		/// <summary>
		/// 获取当前编码的字符串形式
		/// </summary>
		/// <returns></returns>
		public string GetCodex()
		{
			string t = "";
			foreach(Block i in codex.Codex.Keys)
			{
				t += i + " : " + codex.Codex[i] + "\r\n";
			}
			return t;
		}
		/// <summary>
		/// 读取所有字节并解码
		/// </summary>
		/// <returns></returns>
		public Byte[] ReadToEnd()
		{
			List<Byte> result = new List<byte>();
			Block c;
			int bit, i;
			Codec value = new Codec(0, 0);
			Byte temp;
			Byte[] buffer = new byte[4];
			source.Read(buffer, 0, 4);
			int length = Codec.ReadInt(buffer, 4);
			//读取所有字节
			while (source.Position != source.Length && length > 0)
			{
				temp = (Byte)source.ReadByte();
				//对每一个字节的每一位进行匹配
				for (i = 0; i < BitSet.SizeofThis && length > 0; ++i)
				{
					bit = (temp >> (BitSet.SizeofThis - 1 - i)) & 1;
					value.AddBitBack(bit);
					//寻找编码表中该项对应的字符
					if (codex.TryGetKeyByValue(value, out c))
					{
						result.AddRange(c.ToBytes());
						value = new Codec(0, 0);
					}
					--length;
				}
			}
			return result.ToArray();
		}
	}
}
