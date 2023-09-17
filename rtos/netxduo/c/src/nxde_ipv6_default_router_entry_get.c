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
/*    _nxde_ipv6_default_router_entry_get                  PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Yuxin Zhou, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs error checking for the default router entry  */
/*    get service.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ip_ptr                                IP instance pointer           */
/*    interface_index                       Index to the interface        */
/*    router_addr                           Router IPv6 Address           */
/*    router_lifetime                       Pointer to router life time   */
/*    prefix_length                         Pointer to prefix length      */
/*    configuration_method                  Pointer to the information    */
/*                                            on how the entry was        */
/*                                            configured                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nxd_ipv6_default_router_entry_get                                  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application code                                                    */
/*                                                                        */
/*  NOTE                                                                  */
/*                                                                        */
/*                                                                        */
/*    This function cannot be called from ISR.                            */
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
UINT  _nxde_ipv6_default_router_entry_get(NX_IP *ip_ptr, UINT interface_index, UINT entry_index,
                                          NXD_ADDRESS *router_addr, ULONG *router_lifetime,
                                          ULONG *prefix_length, ULONG *configuration_method)
{
#ifdef FEATURE_NX_IPV6

UINT status;


    /* Check for invalid input pointers. */
    if ((ip_ptr == NX_NULL) || (ip_ptr -> nx_ip_id != NX_IP_ID))
    {
        return(NX_PTR_ERROR);
    }

    /* Validate the interface. */
    if (interface_index >= NX_MAX_PHYSICAL_INTERFACES)
    {
        return(NX_INVALID_INTERFACE);
    }

    /* Validate the interface. */
    if (entry_index >= NX_IPV6_DEFAULT_ROUTER_TABLE_SIZE)
    {
        return(NX_INVALID_INTERFACE);
    }

    /* Check for appropriate caller.  */
    NX_INIT_AND_THREADS_CALLER_CHECKING

    /* Call actual IPv6 default router entry get function.  */
    status = _nxd_ipv6_default_router_entry_get(ip_ptr, interface_index, entry_index, router_addr,
                                                router_lifetime, prefix_length, configuration_method);

    /* Return completion status.  */
    return(status);

#else /* !FEATURE_NX_IPV6 */
    NX_PARAMETER_NOT_USED(ip_ptr);
    NX_PARAMETER_NOT_USED(interface_index);
    NX_PARAMETER_NOT_USED(entry_index);
    NX_PARAMETER_NOT_USED(router_addr);
    NX_PARAMETER_NOT_USED(router_lifetime);
    NX_PARAMETER_NOT_USED(prefix_length);
    NX_PARAMETER_NOT_USED(configuration_method);

    return(NX_NOT_SUPPORTED);

#endif /* FEATURE_NX_IPV6 */
}
