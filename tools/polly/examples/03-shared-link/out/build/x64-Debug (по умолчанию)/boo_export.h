
#ifndef BOO_EXPORT_H
#define BOO_EXPORT_H

#ifdef BOO_STATIC_DEFINE
#  define BOO_EXPORT
#  define BOO_NO_EXPORT
#else
#  ifndef BOO_EXPORT
#    ifdef boo_EXPORTS
        /* We are building this library */
#      define BOO_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define BOO_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef BOO_NO_EXPORT
#    define BOO_NO_EXPORT 
#  endif
#endif

#ifndef BOO_DEPRECATED
#  define BOO_DEPRECATED __declspec(deprecated)
#endif

#ifndef BOO_DEPRECATED_EXPORT
#  define BOO_DEPRECATED_EXPORT BOO_EXPORT BOO_DEPRECATED
#endif

#ifndef BOO_DEPRECATED_NO_EXPORT
#  define BOO_DEPRECATED_NO_EXPORT BOO_NO_EXPORT BOO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef BOO_NO_DEPRECATED
#    define BOO_NO_DEPRECATED
#  endif
#endif

#endif /* BOO_EXPORT_H */
