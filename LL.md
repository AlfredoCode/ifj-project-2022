## LL Gramatika - pravidla
	1. <PROG> ---> <PROLOG> <DECLARE>; <ST_L>
	2. <PROLOG> ---> <?php
	3. <DECLARE ---> declare(strict_types=1)
	4. ------------------------------------------------------------
	5. <ST_L> ---> <STAT> <ST_L> 
	6. <ST_L> ---> return    // Jak udělat return data types ve funkcích?
	7. <ST_L> ---> ?>     // NOT GREAT NOT TERRIBLE - V IFECH APOD.
	8. ------------------------------------------------------------
	9. <STAT> ---> $ <ID> <SEPARATOR_LIST>       // Výraz

	11. <STAT> ---> if ( <IF_STAT> <IF_SEP> <IF_STAT> ) { <ST_L> } <STAT_ELSE>   // Podmínka
	13. <STAT> ---> function <ID> ( <FUNC_PARAMS> ) : <DATA_TYPE> { <ST_L> } // TODO ST_L2 - definice vnořených funkcí je zakázána
	14. <STAT> ---> while ( <IF_STAT> <IF_SEP> <IF_STAT> ) { <ST_L> }     // ST_L2 ??
	15. <STAT_ELSE> ---> else { <ST_L> }
	16. 
	17. <IF_STAT> ---> $ <ID> <SEPARATOR>
	18. 
	19. <DATA_TYPE> ---> ? <DATA_TYPE_T>				// Kvuli ?string, ?int apod.
	20. <DATA_TYPE> ---> <DATA_TYPE_T>
	21. 
	22. <DATA_TYPE_T> ---> string
	23. <DATA_TYPE_T> ---> int
	24. <DATA_TYPE_T> ---> float
	25. <DATA_TYPE_T> ---> void
	
	26. 
	27. <FUNC_PARAMS> ---> <DATA_TYPE> $ <ID> <FUNC_PARAM_LIST>
	28. <FUNC_PARAMS> ---> ε     // Funkce může mít 0 parametrů
	29. <FUNC_PARAM_LIST> ---> , <FUNC_PARAMS>

	31. ------------------------------------------------------------
	
	34. 
	35. <IF_SEP> ---> ===
	36. <IF_SEP> ---> !==
	37. <IF_SEP> ---> < 
	38. <IF_SEP> ---> <=
	39. <IF_SEP> ---> >
	40. <IF_SEP> ---> >=
	41. 
	32. <SEPARATOR> ---> . <IF_STAT>
	42. <SEPARATOR> ---> - <IF_STAT>
	43. <SEPARATOR> ---> * <IF_STAT>
	44. <SEPARATOR> ---> + <IF_STAT>
	45. <SEPARATOR> ---> / <IF_STAT>
	48. <SEPARATOR> ---> ;	// Fakt blbost ten semicol, ve funkcích zas bude jebat
	

	49.	<SEPARATOR_LIST> ---> = <IF_STAT>	// Šlo by smazat a přepsat jinam



