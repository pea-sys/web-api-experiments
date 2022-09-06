using DotNetty.Common.Internal.Logging;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WebSockets.Client
{

    using System;
    using DotNetty.Common.Internal.Logging;
    using Microsoft.Extensions.Configuration;
    using Microsoft.Extensions.Logging;

    public static class Helper
    {
        static Helper()
        {
            Configuration = new ConfigurationBuilder()
                .SetBasePath(ProcessDirectory)
                .AddJsonFile("appsettings.json")
                .Build();
        }

        public static string ProcessDirectory
        {
            get
            {
#if NETSTANDARD2_0 || NETCOREAPP3_1_OR_GREATER || NET5_0_OR_GREATER
                return AppContext.BaseDirectory;
#else
                return AppDomain.CurrentDomain.BaseDirectory;
#endif
            }
        }

        public static IConfigurationRoot Configuration { get; }

        public static void SetConsoleLogger() => InternalLoggerFactory.DefaultFactory = LoggerFactory.Create(builder => builder.AddConsole());
    }
}
