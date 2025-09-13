/*
 * Copyright (C) 2025 wolfSSL Inc.
 *
 * This file is part of wolfHSM.
 *
 * wolfHSM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * wolfHSM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with wolfHSM.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * wolfhsm/wh_message_keystore.c
 *
 * Message translation functions for keystore operations.
 */

#include "wolfhsm/wh_message_keystore.h"
#include "wolfhsm/wh_error.h"
#include "wolfhsm/wh_comm.h"
#include <string.h>

/* Key Cache Request translation */
int wh_MessageKeystore_TranslateCacheRequest(
    uint16_t magic, const whMessageKeystore_CacheRequest* src,
    whMessageKeystore_CacheRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, flags);
    WH_T32(magic, dest, src, sz);
    WH_T32(magic, dest, src, labelSz);
    WH_T16(magic, dest, src, id);
    /* Label is just a byte array, no translation needed */
    if (src != dest) {
        memcpy(dest->label, src->label, WH_NVM_LABEL_LEN);
    }
    return 0;
}

/* Key Cache Response translation */
int wh_MessageKeystore_TranslateCacheResponse(
    uint16_t magic, const whMessageKeystore_CacheResponse* src,
    whMessageKeystore_CacheResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Evict Request translation */
int wh_MessageKeystore_TranslateEvictRequest(
    uint16_t magic, const whMessageKeystore_EvictRequest* src,
    whMessageKeystore_EvictRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Evict Response translation */
int wh_MessageKeystore_TranslateEvictResponse(
    uint16_t magic, const whMessageKeystore_EvictResponse* src,
    whMessageKeystore_EvictResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T32(magic, dest, src, ok);
    return 0;
}

/* Key Commit Request translation */
int wh_MessageKeystore_TranslateCommitRequest(
    uint16_t magic, const whMessageKeystore_CommitRequest* src,
    whMessageKeystore_CommitRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Commit Response translation */
int wh_MessageKeystore_TranslateCommitResponse(
    uint16_t magic, const whMessageKeystore_CommitResponse* src,
    whMessageKeystore_CommitResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T32(magic, dest, src, ok);
    return 0;
}

/* Key Export Request translation */
int wh_MessageKeystore_TranslateExportRequest(
    uint16_t magic, const whMessageKeystore_ExportRequest* src,
    whMessageKeystore_ExportRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Export Response translation */
int wh_MessageKeystore_TranslateExportResponse(
    uint16_t magic, const whMessageKeystore_ExportResponse* src,
    whMessageKeystore_ExportResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T32(magic, dest, src, len);
    /* Label is just a byte array, no translation needed */
    if (src != dest) {
        memcpy(dest->label, src->label, WH_NVM_LABEL_LEN);
    }
    return 0;
}

/* Key Erase Request translation */
int wh_MessageKeystore_TranslateEraseRequest(
    uint16_t magic, const whMessageKeystore_EraseRequest* src,
    whMessageKeystore_EraseRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Erase Response translation */
int wh_MessageKeystore_TranslateEraseResponse(
    uint16_t magic, const whMessageKeystore_EraseResponse* src,
    whMessageKeystore_EraseResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T32(magic, dest, src, ok);
    return 0;
}

#ifdef WOLFHSM_CFG_DMA
/*
 * DMA-based keystore operations
 */

/* Key Cache DMA Request translation */
int wh_MessageKeystore_TranslateCacheDmaRequest(
    uint16_t magic, const whMessageKeystore_CacheDmaRequest* src,
    whMessageKeystore_CacheDmaRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T64(magic, dest, src, key.addr);
    WH_T64(magic, dest, src, key.sz);
    WH_T32(magic, dest, src, flags);
    WH_T32(magic, dest, src, labelSz);
    WH_T16(magic, dest, src, id);
    /* Label is just a byte array, no translation needed */
    if (src != dest) {
        memcpy(dest->label, src->label, WH_NVM_LABEL_LEN);
    }
    return 0;
}

/* Key Cache DMA Response translation */
int wh_MessageKeystore_TranslateCacheDmaResponse(
    uint16_t magic, const whMessageKeystore_CacheDmaResponse* src,
    whMessageKeystore_CacheDmaResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T64(magic, dest, src, dmaAddrStatus.badAddr.addr);
    WH_T64(magic, dest, src, dmaAddrStatus.badAddr.sz);
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Export DMA Request translation */
int wh_MessageKeystore_TranslateExportDmaRequest(
    uint16_t magic, const whMessageKeystore_ExportDmaRequest* src,
    whMessageKeystore_ExportDmaRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T64(magic, dest, src, key.addr);
    WH_T64(magic, dest, src, key.sz);
    WH_T16(magic, dest, src, id);
    return 0;
}

/* Key Export DMA Response translation */
int wh_MessageKeystore_TranslateExportDmaResponse(
    uint16_t magic, const whMessageKeystore_ExportDmaResponse* src,
    whMessageKeystore_ExportDmaResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T64(magic, dest, src, dmaAddrStatus.badAddr.addr);
    WH_T64(magic, dest, src, dmaAddrStatus.badAddr.sz);
    WH_T32(magic, dest, src, len);
    /* Label is just a byte array, no translation needed */
    if (src != dest) {
        memcpy(dest->label, src->label, WH_NVM_LABEL_LEN);
    }
    return 0;
}

#endif /* WOLFHSM_CFG_DMA */

#ifdef WOLFHSM_CFG_WRAPKEY
/* Key Wrap Request translation */
int wh_MessageKeystore_TranslateWrapRequest(
    uint16_t magic, const whMessageKeystore_WrapRequest* src,
    whMessageKeystore_WrapRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, keySz);
    WH_T16(magic, dest, src, serverKeyId);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}

/* Key Wrap Response translation */
int wh_MessageKeystore_TranslateWrapResponse(
    uint16_t magic, const whMessageKeystore_WrapResponse* src,
    whMessageKeystore_WrapResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T16(magic, dest, src, wrappedKeySz);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}

/* Key Unwrap Request translation */
int wh_MessageKeystore_TranslateUnwrapAndExportRequest(
    uint16_t magic, const whMessageKeystore_UnwrapAndExportRequest* src,
    whMessageKeystore_UnwrapAndExportRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, wrappedKeySz);
    WH_T16(magic, dest, src, serverKeyId);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}

/* Key Unwrap Response translation */
int wh_MessageKeystore_TranslateUnwrapAndExportResponse(
    uint16_t magic, const whMessageKeystore_UnwrapAndExportResponse* src,
    whMessageKeystore_UnwrapAndExportResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T16(magic, dest, src, keySz);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}

/* Wrapped Key Cache Request translation */
int wh_MessageKeystore_TranslateUnwrapAndCacheRequest(
    uint16_t magic, const whMessageKeystore_UnwrapAndCacheRequest* src,
    whMessageKeystore_UnwrapAndCacheRequest* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T16(magic, dest, src, wrappedKeySz);
    WH_T16(magic, dest, src, serverKeyId);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}

/* Key Cache Response translation */
int wh_MessageKeystore_TranslateUnwrapAndCacheResponse(
    uint16_t magic, const whMessageKeystore_UnwrapAndCacheResponse* src,
    whMessageKeystore_UnwrapAndCacheResponse* dest)
{
    if ((src == NULL) || (dest == NULL)) {
        return WH_ERROR_BADARGS;
    }
    WH_T32(magic, dest, src, rc);
    WH_T16(magic, dest, src, keyId);
    WH_T16(magic, dest, src, cipherType);
    return 0;
}
#endif /* WOLFHSM_CFG_WRAPKEY */
