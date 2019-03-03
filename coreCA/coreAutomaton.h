#pragma once

#include <amp.h>
using namespace concurrency;
#include <time.h>

unsigned long Generate(
	const int d,
	int cMapExt[], const int MapExt, int lMapExt[],
	int Map[],
	const int RuleExt, const int nStates,
	const int cRuleMap[], const int RuleOut[],
	const int nStep = 1)
{
	// Prepare ------------------------

	int *MapIn = new int[MapExt];
	int *MapOut = new int[MapExt];
	for (int i = 0; i < MapExt; i++)
		MapOut[i] = Map[i];

	int *aRuleMap = new int[MapExt*RuleExt];
	int *Coo = new int[d];
	for (int iM = 0; iM < MapExt; iM++)
	{
		int lCoo = iM;
		for (int i = d - 1; i >= 0; i--)
		{
			Coo[i] = lCoo / lMapExt[i];
			lCoo %= lMapExt[i];
		}
		for (int iR = 0; iR < RuleExt; iR++)
		{			
			int _lCoo = 0;
			for (int i = 0; i < d; i++)
				_lCoo += 
					(Coo[i] + cRuleMap[iR*d + i] + cMapExt[i]) % cMapExt[i] 
					* lMapExt[i];
			aRuleMap[iM*RuleExt + iR] = _lCoo;
		}
	}
	delete[] Coo;

	int *Base = new int[RuleExt];
	Base[RuleExt - 1] = 1;
	for (int i = RuleExt - 2; i >= 0; i--)
		Base[i] = Base[i + 1] * nStates;

	// ------------------------
	unsigned long _t = GetTickCount();
	for (int stp = 0; stp < nStep; stp++)
	{
		// copy back
		for (int iM = 0; iM < MapExt; iM++)
			MapIn[iM] = MapOut[iM];

		// Generate
		for (int iM = 0; iM < MapExt; iM++)
		{
			int RuleIn = 0;
			for (int iR = 0; iR < RuleExt; iR++)
				RuleIn += MapIn[aRuleMap[iM*RuleExt + iR]] * Base[iR];
			MapOut[iM] = RuleOut[RuleIn];
		}
	}
	unsigned long t = GetTickCount() - _t;
	delete[] Base;

	// Return
	for (int iM = 0; iM < MapExt; iM++)
		Map[iM] = MapOut[iM];
	return t;
}

unsigned long GenerateWithAMP(
	const int d,
	int cMapExt[], const int MapExt, int lMapExt[],
	int Map[],
	const int RuleExt, const int nStates, const int nRule,
	const int cRuleMap[], const int RuleOut[],
	const int nStep = 1)
{
	// Prepare ------------------------

	concurrency::array<int, 1> AcMapExt(d, cMapExt);
	concurrency::array<int, 1> AlMapExt(d, lMapExt);
	concurrency::array<int, 1> AMapIn(MapExt);
	concurrency::array<int, 1> AMapOut(MapExt, Map);

	concurrency::array<int, 2> AcRuleMap(RuleExt, d, cRuleMap);
	concurrency::array<int, 2> AaRuleMap(MapExt, RuleExt);
	concurrency::array<int, 1> ARuleOut(nRule, RuleOut);
	{
	concurrency::array<int, 2> ACoo(MapExt, d);
	parallel_for_each(AMapOut.extent,
		[=, &AaRuleMap, &ACoo, &AlMapExt, &AcRuleMap, &AcMapExt](index<1> iM) restrict(amp)
	{
		int lCoo = iM[0];
		for (int i = d - 1; i >= 0; i--)
		{
			ACoo(iM[0], i) = lCoo / AlMapExt(i);
			lCoo %= AlMapExt(i);
		}
		for (int iR = 0; iR < RuleExt; iR++)
		{
			int _lCoo = 0;
			for (int i = 0; i < d; i++)
				_lCoo +=
					(ACoo(iM[0], i) + AcRuleMap(iR, i) + AcMapExt(i)) % AcMapExt(i)
					* AlMapExt(i);
			AaRuleMap(iM[0], iR) = _lCoo;
		}
	});
	}

	concurrency::array<int, 1> ABase(RuleExt);
	ABase(RuleExt - 1) = 1;
	for (int i = RuleExt - 2; i >= 0; i--)
		ABase(i) = ABase(i + 1) * nStates;

	// ------------------------
	unsigned long _t = GetTickCount();
	for (int stp = 0; stp < nStep; stp++)
	{
		// copy back
		parallel_for_each(AMapOut.extent,
			[=, &AMapIn, &AMapOut](index<1> iM) restrict(amp)
		{
			AMapIn[iM] = AMapOut[iM];
		});

		// Generate
		parallel_for_each(AMapOut.extent,
			[=, &AMapOut, &AMapIn, &AaRuleMap, &ABase, &ARuleOut](index<1> iM) restrict(amp)
		{
			int RuleIn = 0;
			for (int iR = 0; iR < RuleExt; iR++)
				RuleIn += AMapIn(AaRuleMap(iM[0], iR)) * ABase(iR);
			AMapOut[iM] = ARuleOut(RuleIn);
		});
	}
	unsigned long t = GetTickCount() - _t;

	// Return
	for (int iM = 0; iM < MapExt; iM++)
		Map[iM] = AMapOut(iM);
	return t;
}