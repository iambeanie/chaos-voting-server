using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;
using System;

namespace ChaosHostingApi.Filters
{
    public class AdminFilterAttribute : ActionFilterAttribute
    {
        public AdminFilterAttribute(AppSettings appSettings)
        {
            AppSettings = appSettings ?? throw new ArgumentNullException(nameof(appSettings));
        }

        private AppSettings AppSettings { get; }

        public override void OnActionExecuting(ActionExecutingContext context)
        {
            var suppliedAdminPassword = context.HttpContext.Request.Headers.TryGetValue(nameof(AppSettings.AdminPassword), out var suppliedPassword);

            if (!suppliedAdminPassword || string.IsNullOrWhiteSpace(suppliedPassword) || !suppliedPassword.Equals(AppSettings.AdminPassword))
            {
                context.Result = new OkObjectResult(context.ModelState);
                return;
            }

            base.OnActionExecuting(context);
        }
    }
}
