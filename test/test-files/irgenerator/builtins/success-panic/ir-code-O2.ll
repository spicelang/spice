; ModuleID = 'source.spice'
source_filename = "source.spice"

@str = private unnamed_addr constant [113 x i8] c"Program panicked at ./source.spice:2:5:\0A2  panic(Error(\22This is an error\22));\0A   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\00", align 1

; Function Attrs: noreturn nounwind
define private fastcc void @_Z3foov() unnamed_addr #0 {
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  tail call void @exit(i32 1)
  unreachable
}

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
  tail call fastcc void @_Z3foov() #4
  unreachable
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #3

attributes #0 = { noreturn nounwind }
attributes #1 = { cold noreturn nounwind }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nofree nounwind }
attributes #4 = { nounwind }
