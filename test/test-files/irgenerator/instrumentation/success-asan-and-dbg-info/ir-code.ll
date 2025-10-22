; ModuleID = 'source.spice'
source_filename = "source.spice"

@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr null }]

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !10 {
  %result = alloca i32, align 4
  %iPtr = alloca ptr, align 8
    #dbg_declare(ptr %result, !16, !DIExpression(), !17)
  store i32 0, ptr %result, align 4, !dbg !17
  %1 = call ptr @_Z4sNewIiEPiv(), !dbg !18
  store ptr %1, ptr %iPtr, align 8, !dbg !18
    #dbg_declare(ptr %iPtr, !19, !DIExpression(), !21)
  %2 = load ptr, ptr %iPtr, align 8, !dbg !22
  store i32 123, ptr %2, align 4, !dbg !23
  call void @_Z8sDeallocRPh(ptr %iPtr), !dbg !24
  %3 = load ptr, ptr %iPtr, align 8, !dbg !26
  store i32 321, ptr %3, align 4, !dbg !27
  call void @_Z8sDeallocRPh(ptr %iPtr), !dbg !28
  %4 = load i32, ptr %result, align 4, !dbg !28
  ret i32 %4, !dbg !28
}

declare ptr @_Z4sNewIiEPiv()

declare void @_Z8sDeallocRPh(ptr)

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind uwtable
define internal void @asan.module_ctor() #1 {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind uwtable "frame-pointer"="all" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}
!llvm.dbg.cu = !{!8}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 4, !"nosanitize_address", i32 1}
!7 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!8 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !9, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!9 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-asan-and-dbg-info/source.spice", directory: "./test-files/irgenerator/instrumentation/success-asan-and-dbg-info")
!10 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !11, file: !11, line: 1, type: !12, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !8, retainedNodes: !15)
!11 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-asan-and-dbg-info")
!12 = !DISubroutineType(types: !13)
!13 = !{!14}
!14 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !{}
!16 = !DILocalVariable(name: "result", scope: !10, file: !11, line: 1, type: !14)
!17 = !DILocation(line: 1, column: 1, scope: !10)
!18 = !DILocation(line: 2, column: 22, scope: !10)
!19 = !DILocalVariable(name: "iPtr", scope: !10, file: !11, line: 2, type: !20)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!21 = !DILocation(line: 2, column: 5, scope: !10)
!22 = !DILocation(line: 3, column: 6, scope: !10)
!23 = !DILocation(line: 3, column: 13, scope: !10)
!24 = !DILocation(line: 5, column: 35, scope: !25)
!25 = distinct !DILexicalBlock(scope: !10, file: !11, line: 4, column: 5)
!26 = !DILocation(line: 7, column: 6, scope: !10)
!27 = !DILocation(line: 7, column: 13, scope: !10)
!28 = !DILocation(line: 8, column: 1, scope: !10)
