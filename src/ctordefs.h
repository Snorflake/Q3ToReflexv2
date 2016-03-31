#pragma once

#ifndef CTORDEFS_H
#define CTORDEFS_H

#define NO_COPY_CTOR(CLASSNAME)	\
	CLASSNAME(const CLASSNAME&) = delete;	\
	CLASSNAME& operator= (const CLASSNAME&) = delete;

#define NO_MOVE_CTOR(CLASSNAME) \
	CLASSNAME(CLASSNAME&&) = delete;	\
	CLASSNAME& operator= (CLASSNAME&&) = delete;

#define NO_COPY_OR_MOVE_CTOR(CLASSNAME) NO_COPY_CTOR(CLASSNAME) NO_MOVE_CTOR(CLASSNAME)

#endif
