import { ReactNode } from "react";
import { GridItem } from "@chakra-ui/react";

export default function DashboardMain({ children } : { children : ReactNode}) {
    return (
        <GridItem h="100dvh" borderLeftWidth="1px">
            {children}
        </GridItem>
    )
}