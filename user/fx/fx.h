#ifndef FX_H_
#define FX_H_
#include <esp8266.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define FX_STANDALONE
#define FX_USE_TASK

typedef struct RGB {
	uint8_t		r, g, b;
} __packed RGB;


struct fx;
struct fx {
#define FX_PRIO_CRITICAL			0
#define FX_PRIO_ALERT				1
#define FX_PRIO_WARNING				2
#define FX_PRIO_NORMAL				3
#define FX_PRIO_INFO				4
#define FX_PRIO_DEBUG				5
	int			prio;			// priority is higher if value is smaller. 0 - highest priority
	uint32_t		pattern;		// unique pattern used in effect. If only one bits are repeated
							// in other FX's with higher priority the effect will be skipped.
	const char		*name;			// short name of effect, may be null

	struct style	{

		uint32_t		begin;
		uint32_t		end;

		void			(*start)( void *prv );
		void			(*stop)( void *prv );
		void			(*algo)( void *prv, char *buff, int size );

		void			*prv;		// private data
	}			fx;			//

	struct fx	*next;			// handler to the next effect
};

/**
 * initialize f/x module. Method need to be call at beginning (before using: fx_register, fx_unregister, fx_poll)
 */
void fx_ini(void);

void fx_dei(void);

/**
 * register f/x special effect
 * \param *fx			- handler to fx structure
 * NOTE: is not possible to register one effect twice
 */
void fx_register( struct fx *fx );

/**
 * deregister f/x special effect
 * \param *fx			- handler to fx structure
 */
void fx_deregister( struct fx *fx );

#ifdef	__cplusplus
}
#endif

#endif /* FX_H_ */
