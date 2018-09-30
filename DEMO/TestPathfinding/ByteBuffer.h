#pragma once

#include "Define.h"

namespace BT
{
    class File;

    //----------------------------------------------------------------------------
    /// \brief This object allows to write or read flow of data.
    /// This object allows to write or read flow of data.
    /// The objective is to be very quick so we remove release overflow control.
    /// \code{ .cpp }
    ///     //How to use: Write into file.
    ///     BT::ByteBuffer buf;
    ///     buf << int(13) << 14 << 16;
    ///     BT::File file("myfile.dat");
    ///     file.open(L"w");
    ///     buf.write(file);
    ///     file.close();
    /// \endcode
    class ByteBuffer
    {
        public:
            /// Constructor
            ByteBuffer() = default;
            /// Destructor
            ~ByteBuffer() = default;

            //------------------------------------------------------------------------
            /// \brief  Return how many bytes are allocated into buffer.
            /// 
            /// \returns Allocated bytes into buffer.
            size_t size() const
            {
                return _buffer.size();
            }

            //------------------------------------------------------------------------
            /// \brief  Clean, copy buffer to byteBuffer and allow to read.
            /// 
            /// \param[in] buffer: where read bytes.
            /// \param[in] sizeInByte: how many bytes to read (WARNING: file overflow possible).
            void readBuffer(const void* buffer, const size_t sizeInByte);

            //------------------------------------------------------------------------
            /// \brief  Read bytes from file and copy on cleared buffer.
            /// 
            /// \param[in,out] file: where read bytes (you can seek before).
            /// \param[in] size: how many bytes to read (WARNING: file overflow possible).
            /// \returns Total in Byte of reading data.
            size_t read(File& file, const size_t size);

            //------------------------------------------------------------------------
            /// \brief  Write bytes to file.
            /// 
            /// \param[in,out] file: where write bytes (you can seek before).
            /// \returns Total in Byte of writing data.
            size_t write(File& file) const;
            
            //------------------------------------------------------------------------
            /// \brief Generic writer of byte (possible if sizeof() return number of bytes).
            /// 
            /// \param[in] data: data to save.
            /// \returns New buffer with element.
            template<typename DataType>
            ByteBuffer& operator<<(const DataType& data)
            {
                const MemoryByte* dataA = reinterpret_cast<const MemoryByte*>(&data);
                _buffer.insert(_buffer.end(), dataA, &dataA[sizeof(data)]);
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Write another byte buffer.
            /// 
            /// \param[in] data: data to save.
            /// \returns New buffer with element.
            template<>
            ByteBuffer& operator<<(const ByteBuffer& data)
            {
                _buffer.insert(_buffer.end(), data._buffer.data(), &data._buffer.data()[data._buffer.size()]);
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Reader of generic data.
            /// 
            /// \param[out] data: read data.
            /// \returns New buffer with jump to next element.
            template<typename DataType>
            ByteBuffer& operator>>(DataType& data)
            {
                BT_ASSERT(_readPointer + sizeof(DataType) <= _buffer.size());

                memcpy(&data, reinterpret_cast<const MemoryByte*>(&_buffer[_readPointer]), sizeof(DataType));
                _readPointer += sizeof(DataType);
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Writer of std::vector. Contained type MUST be savable using <<.
            /// 
            /// \param[in] data: data to save.
            /// \returns New buffer with element.
            template<typename DataType>
            ByteBuffer& operator<<(const std::vector<DataType>& data)
            {
                *this << data.size();
                for(const auto& element : data)
                    *this << element;
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Writer of std::pair. Contained type MUST be savable using <<.
            /// 
            /// \param[in] data: data to save.
            /// \returns New buffer with element.
            template<typename DataType, typename DataTypeB>
            ByteBuffer& operator<<(const std::pair<DataType, DataTypeB>& data)
            {
                *this << data.first << data.second;
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Reader of std::vector.
            /// 
            /// \param[out] data: read data.
            /// \returns New buffer with jump to next element.
            template<typename DataType>
            ByteBuffer& operator>>(std::vector<DataType>& data)
            {
                size_t size=0;
                *this >> size;
                // Allocate
                data.resize(size);
                // Fill
                for(auto& element : data)
                    *this >> element;
                return *this;
            }

            //------------------------------------------------------------------------
            /// \brief Reader of std::pair.
            /// 
            /// \param[out] data: read data.
            /// \returns New buffer with jump to next element.
            template<typename DataType, typename DataTypeB>
            ByteBuffer& operator>>(std::pair<DataType, DataTypeB>& data)
            {
                *this >> data.first >> data.second;
                return *this;
            }

        private:
            using MemoryByte  = unsigned char;
            using MemoryArray = std::vector<MemoryByte>;

            MemoryArray _buffer;                ///< Buffer memory.
            std::size_t _readPointer = 0;       ///< Pointer where we are into buffer.
    };

}

