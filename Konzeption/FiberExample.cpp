void CALLBACK foo(LPVOID lpParam)
{
    while (true)
    {
        // Führe die Hauptaufgabe von foo() aus
        DoSomeWork();

        // Wenn eine bestimmte Bedingung erfüllt ist, wechsle zu bar()
        if (SomeConditionMet())
        {
            SwitchToFiber(bar_fiber);
        }
    }
}

void CALLBACK bar(LPVOID lpParam)
{
    // Führe die Aufgabe von bar() aus
    DoSomeOtherWork();

    // Wechsle zurück zu foo()
    SwitchToFiber(foo_fiber);
}

int main()
{
    // Erstelle das Haupt-Fiber
    foo_fiber = CreateFiber(0, foo, nullptr);

    // Erstelle das bar-Fiber
    bar_fiber = CreateFiber(0, bar, nullptr);

    // Starte das Haupt-Fiber
    SwitchToFiber(foo_fiber);

    // Gib die Fibers frei
    DeleteFiber(foo_fiber);
    DeleteFiber(bar_fiber);

    return 0;
}
