// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

namespace Discard.Client
{
    using System.Net;

    public class ClientSettings
    {
        public static bool IsSsl
        {
            get
            {
                string ssl = Helper.Configuration["ssl"];
                return !string.IsNullOrEmpty(ssl) && bool.Parse(ssl);
            }
        }

        public static IPAddress Host => IPAddress.Parse(Helper.Configuration["host"]);

        public static int Port => int.Parse(Helper.Configuration["port"]);

        public static int Size => int.Parse(Helper.Configuration["size"]);

        public static bool UseLibuv
        {
            get
            {
                string libuv = Helper.Configuration["libuv"];
                return !string.IsNullOrEmpty(libuv) && bool.Parse(libuv);
            }
        }
    }
}