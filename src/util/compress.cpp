#include <cstring>
#include <cstdint>
#include <kaldi/base/kaldi-error.h>

#include "compress.h"

// Adapted code from VC_2015_Globus project

namespace globus_internal {

    struct Graph {
        uint16_t vertex[2];
    };

    struct Knot {
        int32_t weight;
        int32_t parent;
    };

    // Pass decompressingBytesLimit to decompress only few first bytes
    uint8_t* decompressRLE(const uint8_t* inbuff, int32_t sizeIn, int32_t& sizeOut) noexcept {
        int32_t i = 0;
        int32_t numBytes = 0;
        int32_t delta;
        uint8_t *outBuff;

        do {
            delta = inbuff[i] & 127;
            if ((inbuff[i] & 128) == 0) {
                i += 2;
            } else {
                i += delta + 1;
            }
            numBytes += delta;
        } while (i < sizeIn);

        outBuff = new uint8_t[numBytes];
        numBytes = 0;
        i = 0;

        do {
            delta = inbuff[i] & 127;
            if ((inbuff[i] & 128) == 0) {
                for (int32_t j = 0; j < delta; ++j) {
                    outBuff[numBytes + j] = inbuff[i + 1];
                }

                i += 2;
            } else {
                for (int32_t j = 0; j < delta; ++j) {
                    outBuff[numBytes + j] = inbuff[i + 1 + j];
                }

                i += delta + 1;
            }
            numBytes += delta;
        } while (i < sizeIn);

        sizeOut = numBytes;
        return outBuff;
    }

    void sort(Knot **pKnot, int left, int right) noexcept {
        int32_t i = left;
        int32_t j = right;
        Knot *comp = pKnot[(left + right) / 2];
        Knot *value;

        do {
            while ((pKnot[i]->weight > comp->weight) && (i < right)) {
                i++;
            }

            while ((pKnot[j]->weight) < comp->weight && (j > left)) {
                j--;
            }

            if (i <= j) {
                value = pKnot[i];
                pKnot[i] = pKnot[j];
                pKnot[j] = value;
                i++;
                j--;
            }
        } while (i <= j);

        if (left < j) {
            sort(pKnot, left, j);
        }

        if (i < right) {
            sort(pKnot, i, right);
        }
    }

    bool getBit(const uint8_t *bits, uint32_t index) noexcept {
        uint8_t bit;
        uint32_t byte;
        byte = index / 8;
        bit = index % 8;
        return (bits[byte] & (1 << bit)) != 0;
    }

    bool createGraph(const uint8_t* table, Graph* graph) noexcept {
        uint16_t mask[256];
        uint8_t  vertex;
        uint16_t prevVertex = 0;

        for (int32_t i = 0; i < 256; ++i) {
            mask[i] = 512;
        }

        for (int32_t i = 0; i < 256; ++i) {
            if (table[i] != 255) {
                prevVertex = i;
                vertex = table[i];

                do {
                    if (mask[vertex] == 512) {
                        mask[vertex] = prevVertex;
                    }

                    graph[vertex].vertex[mask[vertex] != prevVertex] = prevVertex;
                    prevVertex = vertex + 256;
                    vertex = table[prevVertex];

                    if (vertex == prevVertex - 256) {
                        return false;
                    }
                } while (vertex != 255);
            }
        }

        graph[255] = graph[prevVertex - 256];

        return true;
    }

    // Pass decompressingBytesLimit to decompress only few first bytes
    uint8_t* decompressHuffman(uint8_t *inbuff, int32_t sizeIn, int32_t& sizeOut, int32_t decompressingBytesLimit = -1) noexcept {
        int32_t index = 0;
        Graph graph[256], *pGraph;

        if (decompressingBytesLimit != -1) {
            sizeOut = decompressingBytesLimit;
        } else {
            KALDI_ASSERT(sizeIn >= 512);
            sizeOut = *reinterpret_cast<int32_t *>(inbuff + 511);
        }

        uint8_t *outBuff = new uint8_t[sizeOut];

        if (!createGraph(inbuff, graph)) {
            return nullptr;
        }

        inbuff += 511 + 4;

        for (int32_t i = 0; i < sizeOut; ++i) {
            bool b = getBit(inbuff, index++);
            pGraph = &graph[255];

            while (pGraph->vertex[b] > 255) {
                pGraph = &graph[pGraph->vertex[b] - 256];
                b = getBit(inbuff, index++);
            }

            outBuff[i] = (uint8_t) pGraph->vertex[b];
        }

        return outBuff;
    }

    void changeByteOrder(uint8_t* data, int32_t dataSize, int32_t level, bool direction) noexcept {
        uint8_t *data1 = new uint8_t[dataSize];
        int32_t n0 = 0;
        int32_t n1 = 0;

        for (int32_t n = 0; n < level; ++n) {
            n1 = dataSize / level + ((dataSize % level) > n);

            if (direction) {
                for (int32_t i = 0; i < n1; ++i) {
                    data1[n0 + i] = data[i * level + n];
                }
            } else {
                for (int32_t i = 0; i < n1; ++i) {
                    data1[i * level + n] = data[n0 + i];
                }
            }

            n0 += n1;
        }

        std::memcpy(data, data1, dataSize);

        delete[] data1;
    }

    int32_t Histogram::getDataSize() const {
        switch (type >> 16) {
            case 0:
                return numChannels * sizeof(int32_t);
            case 1:
                return numChannels * sizeof(double) * 2;
            case 2:
                return numChannels * sizeof(double) * 3;
            default:
                KALDI_ERR << "Unsupported type";
        }
    }

    Histogram* Histogram::decompress(uint8_t* H, int32_t size) {
        KALDI_ASSERT(H);

        int32_t size0, size1;

        uint8_t* h1 = decompressHuffman(H, size, size1);

        KALDI_ASSERT(h1);

        Histogram* h2 = reinterpret_cast<Histogram*>(decompressRLE(h1, size1, size0));

        KALDI_ASSERT(h2);

        delete[] h1;
        changeByteOrder(reinterpret_cast<uint8_t *>(&h2->data), h2->getDataSize(), 4, false);

        return h2;
    }

    std::string Histogram::decompressName(uint8_t* H) {
        KALDI_ASSERT(H);

        static const constexpr size_t typeAndNameLengthBytes = sizeof(int32_t) + stringFieldsLength;

        int32_t size0, size1;

        uint8_t* h1 = decompressHuffman(H, 0, size1, typeAndNameLengthBytes);

        KALDI_ASSERT(h1);

        uint8_t* h2 = decompressRLE(h1, typeAndNameLengthBytes, size0);

        KALDI_ASSERT(h2);

        delete[] h1;

        return std::string(h2 + sizeof(int32_t), h2 + typeAndNameLengthBytes);
    }
}