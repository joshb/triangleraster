/*
 * Copyright (C) 2009 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include "Color.h"

class Edge
{
	public:
		Color Color1, Color2;
		int X1, Y1, X2, Y2;

		Edge(const Color &color1, int x1, int y1, const Color &color2, int x2, int y2);
};

class Span
{
	public:
		Color Color1, Color2;
		int X1, X2;

		Span(const Color &color1, int x1, const Color &color2, int x2);
};

class Rasterizer
{
	protected:
		uint32_t *m_FrameBuffer;
		unsigned int m_Width, m_Height;

		void DrawSpan(const Span &span, int y);
		void DrawSpansBetweenEdges(const Edge &e1, const Edge &e2);

	public:
		void SetFrameBuffer(uint32_t *frameBuffer, unsigned int width, unsigned int height);
		void SetPixel(unsigned int x, unsigned int y, const Color &color = Color());
		void SetPixel(int x, int y, const Color &color = Color());
		void SetPixel(float x, float y, const Color &color = Color());
		void Clear();

		void DrawTriangle(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2, const Color &color3, float x3, float y3);

		void DrawLine(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2);
};

#endif /* __RASTERIZER_H__ */
