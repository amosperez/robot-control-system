"use client"

import { ReactNode, useState } from "react";
import DashboardSidebar from "./dashboard-sidebar";
import DashboardMain from "./dashboard-main";
import { Grid, Separator } from "@chakra-ui/react";



export default function DashboardShell({ children } : { children : ReactNode}) {
    const [isOpen, setIsOpen] = useState(false);

    const toggleSidebar = () => {
        setIsOpen((previous) => !previous);
    }

    return (
        <Grid templateColumns={isOpen ? "256px minmax(0, 1fr)" : "60px minmax(0, 1fr)"} transition="grid-template-columns .2s ease-in-out">
            <DashboardSidebar onToggle={toggleSidebar}></DashboardSidebar>
            <DashboardMain>
                {children}
            </DashboardMain>
        </Grid>
    )
}