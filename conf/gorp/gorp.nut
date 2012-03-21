--
-- Copyright (C) 2009 by Ole Reinhardt <ole.reinhardt@thermotemp.de>. 
--
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Neither the name of the copyright holders nor the names of
--    contributors may be used to endorse or promote products derived
--    from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
-- COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
-- INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
-- BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
-- OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
-- AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
-- THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.
--
-- For additional information see http://www.ethernut.de/
--

-- Gorp ("Studentenfutter") library
--
-- $Id: gorp.nut 3171 2010-10-15 15:01:06Z astralix $
--

nutgorp =
{
    {
        name = "nutgorp_buffer",
        brief = "Buffer",
        description = "Buffer functions.",
        script = "gorp/buffer.nut"
    },
    {
        name = "nutgorp_edit",
        brief = "Edit",
        description = "Editor functions.",
        script = "gorp/edit.nut"
    },
    {
        name = "nutgorp_codec",
        brief = "Encode and Decode",
        description = "Encoding and decoding functions.",
        script = "gorp/codec.nut"
    },
    {
        name = "nutgorp_hashes",
        brief = "Hashes",
        description = "Hash functions.",
        script = "gorp/hashes.nut"
    },
    {
        name = "nutgorp_list",
        brief = "Lists",
        description = "Functions for lists and queues.",
        script = "gorp/list.nut"
    },
    {
        name = "nutgorp_crypt",
        brief = "Cryptography",
        description = "Functions to encrypt and decrypt data.",
        script = "gorp/crypt.nut"
    }
}
