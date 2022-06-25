using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Caching.Memory;

namespace CachingMemory.Pages
{
    public class IndexModel : PageModel
    {
        private readonly IMemoryCache _memoryCache; 
        public IndexModel(IMemoryCache memoryCache)
        {
            _memoryCache = memoryCache;
        }


        public DateTime CurrentDateTime { get; set; }

        public DateTime SlidingCacheCurrentDateTime { get; set; }

        public DateTime AbsoluteCurrentDateTime { get; set; }

        public DateTime NeverRemoveCurrentDateTime { get; set; }

        public void OnGet()
        {
            CurrentDateTime = DateTime.Now;
            // Sliding Cache
            if (!_memoryCache.TryGetValue(EnumCacheKey.Sliding, out DateTime slidingCacheValue))
            {
                slidingCacheValue = CurrentDateTime;

                var cacheEntryOptions = new MemoryCacheEntryOptions()
                    .SetSlidingExpiration(TimeSpan.FromSeconds(10)).SetSize(16);

                _memoryCache.Set(EnumCacheKey.Sliding, slidingCacheValue, cacheEntryOptions);
            }
            SlidingCacheCurrentDateTime = slidingCacheValue;

            // Absolute Cache
            if (!_memoryCache.TryGetValue(EnumCacheKey.Absolute, out DateTime absoluteCacheValue))
            {
                absoluteCacheValue = CurrentDateTime;

                var cacheEntryOptions = new MemoryCacheEntryOptions()
                    .SetAbsoluteExpiration(TimeSpan.FromSeconds(10)).SetSize(16);

                _memoryCache.Set(EnumCacheKey.Absolute, absoluteCacheValue, cacheEntryOptions);
            }
            AbsoluteCurrentDateTime = absoluteCacheValue;

            // NeverRemove Cache
            if (!_memoryCache.TryGetValue(EnumCacheKey.NeverRemove, out DateTime neverRemoveCacheValue))
            {
                neverRemoveCacheValue = CurrentDateTime;

                var cacheEntryOptions = new MemoryCacheEntryOptions()
                    .SetPriority(CacheItemPriority.NeverRemove).SetSize(16);

                _memoryCache.Set(EnumCacheKey.NeverRemove, neverRemoveCacheValue, cacheEntryOptions);
            }
            NeverRemoveCurrentDateTime = neverRemoveCacheValue;
        }
    }
}