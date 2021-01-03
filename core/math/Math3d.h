#pragma once

#include <string>
#include <algorithm>
#include <initializer_list>

namespace brick {

namespace math{

	template<int Size, typename T>
	class Vector {
	public:
		T m_data[Size] = { 0.0 };

		Vector(){}

		Vector(T data[]) {
			memcpy(m_data, data, sizeof(T) * Size);
		}

		T& operator[](int idx) {
			return m_data[idx];
		}

		T operator[](int idx) const {
			return m_data[idx];
		}

		template<int Size2>
		void operator = (const Vector<Size2, T>& other) {
			constexpr int MinSize = std::min(Size, Size2);
			for (int i = 0; i < MinSize; i++) {
				m_data[i] = other[i];
			}
		}

		Vector operator * (float scaler) const {
			T data[Size];
			for (int i = 0; i < Size; i++) 
				data[i] = m_data[i] * scaler;
			return data;
		}

		Vector operator + (float scaler) const {
			T data[Size];
			for (int i = 0; i < Size; i++) 
				data[i] = m_data[i] + scaler;
			return data;
		}

		//template<int Size2, typename T2>
		//Vector operator + (const Vector<Size2, T2>& vec) const {
		//	constexpr int MaxSize = std::max(Size, Size2);
		//	T data[MaxSize];
		//	if constexpr (Size > Size2) {
		//		for (int i = 0; i < Size2; i++) 
		//			data[i] = m_data[i] + vec[i];
		//		for(int i = Size2; i < Size; i++)
		//			data[i] = m_data[i]
		//	}
		//	else {
		//		for (int i = 0; i < Size; i++) 
		//			data[i] = m_data[i] + vec[i];
		//		for(int i = Size; i < Size2; i++)
		//			data[i] = vec[i]
		//	}
		//	return data;
		//}

		Vector operator + (const Vector& vec) const {
			constexpr int MaxSize = std::max(Size, Size2);
			T data[Size];
			for (int i = 0; i < Size; i++)
				data[i] = m_data[i] + vec[i];
			return data;
		}

		T Dot(const Vector& vec) const {
			T Result = 0.0;
			for (int i = 0; i < Size; i++)
				Result += m_data[i] * vec[i];
			return Result;
		}

		//Vector Cross(const Vector& other) const {
		//	static_assert(false);
		//}

	};

	template<>
	class Vector<3, float> {
	public:
		union{
			struct{
				float m_data[3];
			};
			struct {
				float x, y, z;
			};
			struct {
				float r, g, b;
			};
		};

		Vector() {
			x = y = z = 0.0f;
		}

		Vector(float s) {
			x = y = z = s;
		}

		Vector(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector(float data[3]) {
			memcpy(m_data, data, sizeof(float) * 3);
		}

		float& operator[](int idx) {
			return m_data[idx];
		}

		float operator[](int idx) const {
			return m_data[idx];
		}

		Vector Cross(const Vector& other) const {
			return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
		}

		Vector operator + (float scaler) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] + scaler;
			return data;
		}

		Vector operator + (const Vector& vec) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] + vec[i];
			return data;
		}

		Vector operator * (float scaler) const {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = m_data[i] * scaler;
			return data;
		}

		friend Vector operator * (float scaler, const Vector& vec) {
			float data[3];
			for (int i = 0; i < 3; i++)
				data[i] = vec[i] * scaler;
			return data;
		}

		std::string GetString() {
			std::string Result = "";
			char Buf[64];
			for (int i = 0; i < 3; i++) {
				sprintf_s(Buf, "%f", m_data[i]);
				Result += Buf;
				Result += " ";
			}
			return Result;
		}

		const float* GetData() {
			return m_data;
		}

	};

	//column major
	template<int N, int M, typename T>
	class Matrix {
	public:
		
		static Matrix Identity() {
			//constexpr int V = std::min(N, M);
			int V = std::min<int>(N, M);
			T data[N * M] = { 0.0f };
			for (int i = 0; i < V; i++) {
				data[i * M + i] = 1.0f;
			}
			return data;
		}

		Matrix(){}

		Matrix(const T data[]) {
			//static_assert(sizeof(data) == N * M * sizeof(T));
			int a = sizeof(T) * N * M;
			memcpy(m_data, data, sizeof(T) * N * M);
		}

		Matrix(std::initializer_list<T> args) {
			int i = 0;
			for (auto iter = args.begin(); iter != args.end(); ++iter, i++)
				m_data[i] = *iter;
		}

		T* operator[](int n) {
			return (T*)&m_data[n * M];
		}

		T& operator() (int n, int c) {
			return m_data[n * M + c];
		}

		T operator() (int n, int c) const {
			return m_data[n * M + c];
		}
			
		template<int M2>
		Matrix operator * (const Matrix<M, M2, T>& other) {
			const T* other_data = other.GetData();
			T data[N * M2] = { 0.0 };
			for (int r = 0; r < N; r++)
				for (int c = 0; c < M; c++) {
					int idx = r * M + c;
					for (int i = 0; i < M; i++)
						data[idx] += m_data[r * M + i] * other_data[i * M2 + c];
				}
			return data;
		}

		template<int TN>
		Matrix<TN, TN, T> ToSquare() {
			T data[TN * TN] = { 0.0 };
			int Rows = std::min<int>(TN, N);
			int Cols = std::min<int>(TN, M);
			for (int r = 0; r < Rows; r++)
				for (int c = 0; c < Cols; c++) {
					data[r * TN + c] = m_data[r * M + c];
				}
			int S = std::min<int>(Rows, Cols);
			for (int i = S; i < TN; i++)
				data[i * M + i] = 1.0f;
			return data;
		}

		Vector<N, T> operator * (const Vector<M, T> other) {
			T data[N] = {0.0};
			for(int r = 0; r < N; r++)
				for (int c = 0; c < M; c++) {
					data[r] += m_data[r * M + c] * other[c];
				}
			return data;
		}

		Matrix<M, N, T> Transpose() {
			T data[N * M] = { 0.0f };
			for (int r = 0; r < M; r++) 
				for (int c = 0; c < N; c++) {
					data[r * N + c] = m_data[c * M + r];
				}
			return data;
		}

		std::string GetString() {
			std::string Result = "";
			char Buf[64];
			for (int r = 0; r < N; r++) {
				for (int c = 0; c < M; c++) {
					sprintf(Buf, "%f", m_data[c * M + r]);
					Result += Buf;
					Result += " ";
				}
				Result += "\n";
			}
			return Result;
		}

		const T* GetData() const {
			return m_data;
		}
	private:
		T m_data[N * M] = { 0.0 };
	};

	template<typename T> using Vector3 = Vector<3, T>;
	template<typename T> using Vector4 = Vector<4, T>;
	using Vector3f = Vector3<float>;
	using Vector4f = Vector4<float>;
	
	template<typename T> using Matrix3x3 = Matrix<3, 3, T>;
	template<typename T> using Matrix4x4 = Matrix<4, 4, T>;
	using Matrix4x4f = Matrix4x4<float>;

	template<typename T>
	class Quaternion {
	public:
		Quaternion() {
			SetIdentity();
		}
		Quaternion(Vector3<T> axis, T angle) {
			// 约束：axis必须被归一化
			T half_angle = (T)(0.5 * angle);
			T sin_value = sin(half_angle);

			x = axis.x * sin_value;
			y = axis.y * sin_value;
			z = axis.z * sin_value;
			w = cos(half_angle);
		}
	
		void SetIdentity()
		{
			Set(0, 0, 0, 1);
		}

		void Init(T _x = 0, T _y = 0, T _z = 0, T _w = 0)
		{
			Set(_x, _y, _z, _w);
		}

		void Set(T _x, T _y, T _z, T _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		void GetAxisAngle(Vector3<T>& axis, T& angle)
		{
			// 约束：四元数必须被归一化
			T half_angle = (T)acos(w);
			angle = half_angle * 2;
			T sin_value = (T)sin(half_angle);
			axis.Set(x / sin_value, y / sin_value, z / sin_value);
		}

		void Negative()
		{
			for (int i = 0; i < 4; ++i)
			{
				q[i] = -q[i];
			}
		}

		void ToRotationMatrix(Matrix3x3<T>& Rot) const
		{
			T fTx = x + x;
			T fTy = y + y;
			T fTz = z + z;
			T fTwx = fTx * w;
			T fTwy = fTy * w;
			T fTwz = fTz * w;
			T fTxx = fTx * x;
			T fTxy = fTy * x;
			T fTxz = fTz * x;
			T fTyy = fTy * y;
			T fTyz = fTz * y;
			T fTzz = fTz * z;

			Rot[0][0] = 1.0f - (fTyy + fTzz);
			Rot[1][0] = fTxy - fTwz;
			Rot[2][0] = fTxz + fTwy;
			Rot[0][1] = fTxy + fTwz;
			Rot[1][1] = 1.0f - (fTxx + fTzz);
			Rot[2][1] = fTyz - fTwx;
			Rot[0][2] = fTxz - fTwy;
			Rot[1][2] = fTyz + fTwx;
			Rot[2][2] = 1.0f - (fTxx + fTyy);
		}

		Matrix3x3<T> GetRotationMatrix() const {
			Matrix3x3<T> Mat;
			ToRotationMatrix(Mat);
			return Mat;
		}


	private:
		union
		{
			T q[4];
			struct
			{
				T x, y, z, w;
			};
		};

	};
	
	using Quaternionf = Quaternion<float>;


}

}

