using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System.Text;
using Microsoft.Azure.Devices.Client;
using Microsoft.Azure.Devices;
using HtmlAgilityPack;

namespace arduinoAPI.Controllers
{
    [Route("arduino/[controller]")]
    [ApiController]
    public class ArduinoController : ControllerBase
    {

        [HttpPost]
        public IActionResult SendMessage(string messageContent)
        {
            var htmlDoc = new HtmlDocument();
            htmlDoc.LoadHtml(messageContent);

            var textNode = htmlDoc.DocumentNode.SelectSingleNode("//div[@dir='ltr']");


            var serviceClientStr = "HostName=arduino2.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=6LnpxKS1tg2h3c3m2UDL7BzNlW0fxof8dAIoTNSMcKg=";

            //sned a new message from could to device
            var serviceClient = ServiceClient.CreateFromConnectionString(serviceClientStr);
            var commandMessage = new Microsoft.Azure.Devices.Message(Encoding.ASCII.GetBytes(textNode.ToString()));

            serviceClient.SendAsync("testarduino2", commandMessage).GetAwaiter().GetResult();
            Console.WriteLine("sent message to device,content : " + textNode);
            return Ok("test");
        }
    }
}
