#pragma once

#ifdef CLOCKWORK_EXPORTS
#define CLOCKWORK_API __declspec(dllexport)
#else
#define CLOCKWORK_API __declspec(dllimport)
#endif