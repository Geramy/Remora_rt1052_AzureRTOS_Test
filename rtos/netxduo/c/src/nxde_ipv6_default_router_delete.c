/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Component                                                        */
/**                                                                       */
/**   Internet Protocol version 6 Default Router Table (IPv6 router)      */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/
#define NX_SOURCE_CODE


/* Include necessary system files.  */

#include "../../c/inc/nx_api.h"
#include "../../c/inc/nx_ipv6.h"
#ifdef FEATURE_NX_IPV6
#include "../../c/inc/nx_ip.h"
#include "../../c/inc/nx_nd_cache.h"

/* Bring in externs for caller checking code.  */
NX_CALLER_CHECKING_EXTERNS

#endif /* FEATURE_NX_IPV6 */



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nxde_ipv6_default_router_delete                     PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Yuxin Zhou, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs error checking on the routing table entry    */
/*    delete service.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ip_ptr                                Pointer to IP control block   */
/*    router_address                        router  IP address            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    NX_PTR_ERROR                          Invalid pointer input         */
/*    NX_INVALID_PARAMETERS                 Invalid IP address version    */
/*    status                                Actual completion Status      */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nxd_ipv6_default_router_delete       Actual router delete service  */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Yuxin Zhou               Initial Version 6.0           */
/*  09-30-2020     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _nxde_ipv6_default_router_delete(NX_IP *ip_ptr, NXD_ADDRESS *router_address)
{
#ifdef FEATURE_NX_IPV6

    /* Check for invalid input pointers. */
    if ((ip_ptr == NX_NULL) || (ip_ptr -> nx_ip_id != NX_IP_ID) || (router_address == NX_NULL))
    {
        return(NX_PTR_ERROR);
    }

    /* Must be in IPv6 address type. */
    if (router_address -> nxd_ip_version != NX_IP_VERSION_V6)
    {
        return(NX_INVALID_PARAMETERS);
    }

    /* Check for appropriate caller.  */
    NX_INIT_AND_THREADS_CALLER_CHECKING

    /* Call the actual router delete service and return completion status. */
    return(_nxd_ipv6_default_router_delete(ip_ptr, router_address));

#else /* !FEATURE_NX_IPV6 */
    NX_PARAMETER_NOT_USED(ip_ptr);
    NX_PARAMETER_NOT_USED(router_address);

    return(NX_NOT_SUPPORTED);

#endif /* FEATURE_NX_IPV6 */
}

