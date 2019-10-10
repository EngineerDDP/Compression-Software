using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanCodec.Codex
{
	/// <summary>
	/// 哈夫曼编码表元素
	/// </summary>
	class Codec
	{
		public int value;
		public Byte length { get; private set; }
		private static Int32 ValueLength = Block.BlockSize + 1;
		public static Int32 SizeofThis = ValueLength + 1;

		/// <summary>
		/// 通过Serialize的Byte数组还原节点元素
		/// </summary>
		public Codec(Byte[] bytes)
		{
			length = bytes[0];
			value = ReadInt(bytes.Skip(1).ToArray(), ValueLength);
		}
		/// <summary>
		/// 通过给定的不等长编码和编码长度创建表元素
		/// </summary>
		public Codec(int value, byte length)
		{
			this.value = value;
			this.length = length;
		}
		/// <summary>
		/// 在编码前添加一位
		/// </summary>
		public void AddBitFront(int bit)
		{
			if (length == ValueLength * 8)
				throw new ArgumentOutOfRangeException("位字长不足以容纳指定的bit值");
			else
			{
				value = value | (bit << length);
				++length;
			}
		}
		/// <summary>
		/// 在编码后面添加一位
		/// </summary>
		/// <param name="bit"></param>
		public void AddBitBack(int bit)
		{
			if (length == ValueLength * 8)
				throw new ArgumentOutOfRangeException("位字长不足以容纳指定的bit值");
			else
			{
				value = (value << 1) | bit;
				++length;
			}
		}
		/// <summary>
		/// 转换到Byte数组
		/// </summary>
		public Byte[] ToBytes()
		{
			Byte[] temp = new Byte[SizeofThis];
			temp[0] = length;
			WriteInt(value, ValueLength).CopyTo(temp, 1);
			return temp;
		}
		/// <summary>
		/// 从Byte数组中读取int
		/// </summary>
		public static int ReadInt(Byte[] input, int Length)
		{
			int temp = 0;
			for (var i = Length - 1; i >= 0; --i)
			{
				temp += input[Length - i - 1] << (8 * i);
			}
			return temp;
		}
		/// <summary>
		/// 将Int写入Byte数组
		/// </summary>
		public static Byte[] WriteInt(int input, int length)
		{
			Byte[] temp = new Byte[length];
			for (var i = length - 1; i >= 0; --i)
			{
				temp[length - i - 1] = (Byte)(input >> (8 * i));
			}
			return temp;
		}
		/// <summary>
		/// 转字符串
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			return Convert.ToString(value, 2).PadLeft(length, '0');
		}
		/// <summary>
		/// 比较相等
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		public override bool Equals(object obj)
		{
			if (obj.GetType() == this.GetType())
			{
				Codec t = (Codec)obj;
				if (t.length == length && t.value == value)
					return true;
			}
			return false;
		}
		public override int GetHashCode()
		{
			return value | (1 << length);
		}
	}
}
