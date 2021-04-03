using ChaosHostingApi.Filters;
using ChaosHostingApi.Hubs;
using ChaosHostingApi.Services;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.OpenApi.Models;

namespace ChaosHostingApi
{
    public class Startup
    {
        private readonly string CorsPolicy = "ChaosCors";

        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        public void ConfigureServices(IServiceCollection services)
        {
            var appSettings = new AppSettings();

            Configuration.GetSection(nameof(AppSettings)).Bind(appSettings);

            services.AddSingleton(appSettings);

            services.AddScoped<AdminFilterAttribute>();
            services.AddSingleton<ChaosGameManager>();

            services.AddControllers();
            services.AddSwaggerGen(c =>
            {
                c.SwaggerDoc("v1", new OpenApiInfo { Title = "ChaosHostingApi", Version = "v1" });
            });

            services.AddSignalR();

            services.AddCors(options =>
            {
                options.AddPolicy(name: CorsPolicy,
                builder =>
                {
                    builder.WithOrigins(appSettings.UIUrl)
                        .AllowCredentials()
                        .AllowAnyHeader()
                        .AllowAnyMethod();
                });
            });
        }

        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
                app.UseSwagger();
                app.UseSwaggerUI(c => c.SwaggerEndpoint("/swagger/v1/swagger.json", "ChaosHostingApi v1"));
            }

            app.UseHttpsRedirection();

            app.UseRouting();

            app.UseAuthorization();

            app.UseCors(CorsPolicy);

            app.UseWebSockets();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapHub<VotingHub>("/votinghub");
                endpoints.MapHub<HostingHub>("/hostinghub");
                endpoints.MapControllers();
            });
        }
    }
}