using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Discard.Server
{
    using System;
    using DotNetty.Buffers;
    using DotNetty.Transport.Channels;

    public class DiscardServerHandler : SimpleChannelInboundHandler<object>
    {
        protected override void ChannelRead0(IChannelHandlerContext context, object message)
        {
            var buffer = message as IByteBuffer;
            if (buffer != null)
            {
                Console.WriteLine("Received from client: " + buffer.ToString(Encoding.UTF8));
            }
        }

        public override void ExceptionCaught(IChannelHandlerContext ctx, Exception e)
        {
            Console.WriteLine("{0}", e.ToString());
            ctx.CloseAsync();
        }
    }
}
