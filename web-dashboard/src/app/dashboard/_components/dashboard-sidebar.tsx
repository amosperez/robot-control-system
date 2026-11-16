import { GridItem, IconButton, Button, Flex, Center, Box} from "@chakra-ui/react"
import { LuPanelLeftOpen, LuSettings, LuHouse, LuBot} from "react-icons/lu"
import Link from "next/link";
import { ReactNode } from "react";

interface navItemShape {
    title: string,
    href: string,
    icon: ReactNode
}

const navItems = [
    {
        title: "Dashboard",
        href: "/dashboard",
        icon: <LuHouse />
    },
    {
        title: "Robots",
        href: "/dashboard/robots",
        icon: <LuBot />
    }
]

interface DashboardSidebarProps {
    onToggle: () => void,
}

export default function DashboardSidebar({ onToggle } : DashboardSidebarProps) {
    return (
        <GridItem h="100dvh">
            <Flex direction="column" align="start" p="2" h="100%">
                <IconButton variant="ghost" size="lg" onClick={onToggle}>
                    <LuPanelLeftOpen />
                </IconButton>
                <NavItems navItems={navItems} />
                <Box mt="auto" w="full"><NavItem title="Settings" href="/dashboard/settings" icon={<LuSettings />}/></Box>
            </Flex>
        </GridItem>
    )
}

function NavItem({ title, href, icon } : { title: string, href: string, icon: ReactNode}) {
    return <Button asChild variant="ghost" size="lg" pl="0" pr="0" aria-label={title} w="full" justifyContent="start" overflow="hidden" position="relative">
        <Link href={href}>
            <Center w="42px" h="42px">{icon}</Center>
            <Box position="absolute" left="42px">
                {title}
            </Box>
        </Link>
    </Button>
}

function NavItems({ navItems } : { navItems: navItemShape[] }) {
    return navItems.map((item) => (
        <NavItem key={item.href} title={item.title} href={item.href} icon={item.icon} />
    ))
}