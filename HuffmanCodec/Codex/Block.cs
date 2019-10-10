using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanCodec.Codex
{
	class Block
	{
		private int value;
		public static int BlockSize = 2;
		/// <summary>
		/// 默认构造方法
		/// </summary>
		public Block(System.IO.Stream s)
		{
			Init(s);
		}
		/// <summary>
		/// 用字节数组生成Block
		/// </summary>
		/// <param name="buffer"></param>
		public Block(Byte[] buffer)
		{
			Init(buffer);
		}
		/// <summary>
		/// 从流中获取数据并初始化Block
		/// </summary>
		/// <param name="s"></param>
		/// <exception cref="DataMisalignedException"></exception>
		private void Init(System.IO.Stream s)
		{
			byte[] buffer = new byte[BlockSize];
			if (s.Length - s.Position < BlockSize)
				throw new DataMisalignedException();
			else
			{
				s.Read(buffer, 0, BlockSize);
				value = Codec.ReadInt(buffer, BlockSize);
			}
		}
		/// <summary>
		/// 从字节数组中获取数据并初始化Block
		/// </summary>
		/// <param name="s"></param>
		/// <exception cref="DataMisalignedException"></exception>
		private void Init(Byte[] buffer)
		{
			if(buffer.Length < BlockSize)
				throw new DataMisalignedException();
			else
				value = Codec.ReadInt(buffer, BlockSize);
		}
		/// <summary>
		/// 将Block转换为字节数组
		/// </summary>
		/// <returns></returns>
		public Byte[] ToBytes()
		{
			return Codec.WriteInt(value, BlockSize);
		}
		/// <summary>
		/// 对比相等
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj)
		{
			if (obj.GetType() == GetType())
			{
				Block i = (Block)obj;
				if (value == i.value)
					return true;
			}
			return false;
		}
		public override int GetHashCode()
		{
			return value;
		}
		/// <summary>
		/// 调试用
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			return value.ToString();
		}
	}
}
