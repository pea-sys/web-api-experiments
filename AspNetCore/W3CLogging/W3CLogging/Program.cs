using Microsoft.AspNetCore.HttpLogging;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddW3CLogging(logging =>
{
    //// Log all W3C fields
    logging.LoggingFields = W3CLoggingFields.All;

    logging.FileSizeLimit = 5 * 1024 * 1024;
    logging.RetainedFileCountLimit = 2;
    //logging.FileName = "MyLogFile";//�t�@�C�������w�肵�Ȃ���w3clog-yyyyMMdd.xxxx.txt�ɂȂ�(xxxx�͋N������x�ɍ̔Ԃ������ۂ�)
    //logging.LogDirectory = @"C:\logs";//�t�H���_���w�肵�Ȃ���exe�t�@�C���̂P���logs�t�H���_�������
    logging.FlushInterval = TimeSpan.FromSeconds(2);
});
// Add services to the container.

var app = builder.Build();

app.UseW3CLogging(); //W3C���M���O�~�h���̎g�p�錾
// Configure the HTTP request pipeline.

var summaries = new[]
{
    "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
};

app.MapGet("/weatherforecast", () =>
{
    var forecast = Enumerable.Range(1, 5).Select(index =>
        new WeatherForecast
        (
            DateTime.Now.AddDays(index),
            Random.Shared.Next(-20, 55),
            summaries[Random.Shared.Next(summaries.Length)]
        ))
        .ToArray();
    return forecast;
});

app.Run();

internal record WeatherForecast(DateTime Date, int TemperatureC, string? Summary)
{
    public int TemperatureF => 32 + (int)(TemperatureC / 0.5556);
}