using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebSockets.Server
{
    internal class ServerSettings
    {
        public static bool IsSsl
        {
            get
            {
                string ssl = Helper.Configuration["ssl"];
                return !string.IsNullOrEmpty(ssl) && bool.Parse(ssl);
            }
        }

        public static int Port => int.Parse(Helper.Configuration["port"]);

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
