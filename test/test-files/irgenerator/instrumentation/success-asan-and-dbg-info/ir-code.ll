; ModuleID = 'source.spice'
source_filename = "source.spice"

$asan.module_ctor = comdat any

@__asan_option_detect_stack_use_after_return = external global i32
@___asan_gen_stack = private unnamed_addr constant [14 x i8] c"1 32 8 4 iPtr\00", align 1
@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@___asan_globals_registered = common hidden global i64 0
@__start_asan_globals = extern_weak hidden global i64
@__stop_asan_globals = extern_weak hidden global i64
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr @asan.module_ctor }]

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_address uwtable
define dso_local noundef i32 @main() #0 !dbg !10 {
  %result = alloca i32, align 4
  %asan_local_stack_base = alloca i64, align 8
  %1 = load i32, ptr @__asan_option_detect_stack_use_after_return, align 4
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %5

3:                                                ; preds = %0
  %4 = call i64 @__asan_stack_malloc_0(i64 64)
  br label %5

5:                                                ; preds = %0, %3
  %6 = phi i64 [ 0, %0 ], [ %4, %3 ]
  %7 = icmp eq i64 %6, 0
  br i1 %7, label %8, label %10

8:                                                ; preds = %5
  %MyAlloca = alloca i8, i64 64, align 32
  %9 = ptrtoint ptr %MyAlloca to i64
  br label %10

10:                                               ; preds = %5, %8
  %11 = phi i64 [ %6, %5 ], [ %9, %8 ]
  store i64 %11, ptr %asan_local_stack_base, align 8
  %12 = add i64 %11, 32
  %13 = inttoptr i64 %12 to ptr
  %14 = inttoptr i64 %11 to ptr
  store i64 1102416563, ptr %14, align 8
  %15 = add i64 %11, 8
  %16 = inttoptr i64 %15 to ptr
  store i64 ptrtoint (ptr @___asan_gen_stack to i64), ptr %16, align 8
  %17 = add i64 %11, 16
  %18 = inttoptr i64 %17 to ptr
  store i64 ptrtoint (ptr @main to i64), ptr %18, align 8
  %19 = lshr i64 %11, 3
  %20 = add i64 %19, 2147450880
  %21 = add i64 %20, 0
  %22 = inttoptr i64 %21 to ptr
  store i64 -868083117767659023, ptr %22, align 1
    #dbg_declare(ptr %result, !16, !DIExpression(), !17)
  store i32 0, ptr %result, align 4, !dbg !17
  %23 = call ptr @_Z4sNewIiEPiv(), !dbg !18
  store ptr %23, ptr %13, align 8, !dbg !18
    #dbg_declare(ptr %asan_local_stack_base, !19, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 32), !21)
  %24 = load ptr, ptr %13, align 8, !dbg !22
  %25 = ptrtoint ptr %24 to i64, !dbg !23
  %26 = lshr i64 %25, 3, !dbg !23
  %27 = add i64 %26, 2147450880, !dbg !23
  %28 = inttoptr i64 %27 to ptr, !dbg !23
  %29 = load i8, ptr %28, align 1, !dbg !23
  %30 = icmp ne i8 %29, 0, !dbg !23
  br i1 %30, label %31, label %37, !dbg !23, !prof !24

31:                                               ; preds = %10
  %32 = and i64 %25, 7, !dbg !23
  %33 = add i64 %32, 3, !dbg !23
  %34 = trunc i64 %33 to i8, !dbg !23
  %35 = icmp sge i8 %34, %29, !dbg !23
  br i1 %35, label %36, label %37, !dbg !23

36:                                               ; preds = %31
  call void @__asan_report_store4(i64 %25) #3, !dbg !23
  unreachable

37:                                               ; preds = %31, %10
  store i32 123, ptr %24, align 4, !dbg !23
  call void @_Z8sDeallocRPh(ptr %13), !dbg !25
  %38 = load ptr, ptr %13, align 8, !dbg !27
  %39 = ptrtoint ptr %38 to i64, !dbg !28
  %40 = lshr i64 %39, 3, !dbg !28
  %41 = add i64 %40, 2147450880, !dbg !28
  %42 = inttoptr i64 %41 to ptr, !dbg !28
  %43 = load i8, ptr %42, align 1, !dbg !28
  %44 = icmp ne i8 %43, 0, !dbg !28
  br i1 %44, label %45, label %51, !dbg !28, !prof !24

45:                                               ; preds = %37
  %46 = and i64 %39, 7, !dbg !28
  %47 = add i64 %46, 3, !dbg !28
  %48 = trunc i64 %47 to i8, !dbg !28
  %49 = icmp sge i8 %48, %43, !dbg !28
  br i1 %49, label %50, label %51, !dbg !28

50:                                               ; preds = %45
  call void @__asan_report_store4(i64 %39) #3, !dbg !28
  unreachable

51:                                               ; preds = %45, %37
  store i32 321, ptr %38, align 4, !dbg !28
  call void @_Z8sDeallocRPh(ptr %13), !dbg !29
  %52 = load i32, ptr %result, align 4, !dbg !29
  store i64 1172321806, ptr %14, align 8, !dbg !29
  %53 = icmp ne i64 %6, 0, !dbg !29
  br i1 %53, label %54, label %61, !dbg !29

54:                                               ; preds = %51
  %55 = add i64 %20, 0, !dbg !29
  %56 = inttoptr i64 %55 to ptr, !dbg !29
  store i64 -723401728380766731, ptr %56, align 1, !dbg !29
  %57 = add i64 %6, 56, !dbg !29
  %58 = inttoptr i64 %57 to ptr, !dbg !29
  %59 = load i64, ptr %58, align 8, !dbg !29
  %60 = inttoptr i64 %59 to ptr, !dbg !29
  store i8 0, ptr %60, align 1, !dbg !29
  br label %64, !dbg !29

61:                                               ; preds = %51
  %62 = add i64 %20, 0, !dbg !29
  %63 = inttoptr i64 %62 to ptr, !dbg !29
  store i64 0, ptr %63, align 1, !dbg !29
  br label %64, !dbg !29

64:                                               ; preds = %61, %54
  ret i32 %52, !dbg !29
}

declare ptr @_Z4sNewIiEPiv()

declare void @_Z8sDeallocRPh(ptr)

declare void @__asan_report_load_n(i64, i64)

declare void @__asan_loadN(i64, i64)

declare void @__asan_report_load1(i64)

declare void @__asan_load1(i64)

declare void @__asan_report_load2(i64)

declare void @__asan_load2(i64)

declare void @__asan_report_load4(i64)

declare void @__asan_load4(i64)

declare void @__asan_report_load8(i64)

declare void @__asan_load8(i64)

declare void @__asan_report_load16(i64)

declare void @__asan_load16(i64)

declare void @__asan_report_store_n(i64, i64)

declare void @__asan_storeN(i64, i64)

declare void @__asan_report_store1(i64)

declare void @__asan_store1(i64)

declare void @__asan_report_store2(i64)

declare void @__asan_store2(i64)

declare void @__asan_report_store4(i64)

declare void @__asan_store4(i64)

declare void @__asan_report_store8(i64)

declare void @__asan_store8(i64)

declare void @__asan_report_store16(i64)

declare void @__asan_store16(i64)

declare void @__asan_report_exp_load_n(i64, i64, i32)

declare void @__asan_exp_loadN(i64, i64, i32)

declare void @__asan_report_exp_load1(i64, i32)

declare void @__asan_exp_load1(i64, i32)

declare void @__asan_report_exp_load2(i64, i32)

declare void @__asan_exp_load2(i64, i32)

declare void @__asan_report_exp_load4(i64, i32)

declare void @__asan_exp_load4(i64, i32)

declare void @__asan_report_exp_load8(i64, i32)

declare void @__asan_exp_load8(i64, i32)

declare void @__asan_report_exp_load16(i64, i32)

declare void @__asan_exp_load16(i64, i32)

declare void @__asan_report_exp_store_n(i64, i64, i32)

declare void @__asan_exp_storeN(i64, i64, i32)

declare void @__asan_report_exp_store1(i64, i32)

declare void @__asan_exp_store1(i64, i32)

declare void @__asan_report_exp_store2(i64, i32)

declare void @__asan_exp_store2(i64, i32)

declare void @__asan_report_exp_store4(i64, i32)

declare void @__asan_exp_store4(i64, i32)

declare void @__asan_report_exp_store8(i64, i32)

declare void @__asan_exp_store8(i64, i32)

declare void @__asan_report_exp_store16(i64, i32)

declare void @__asan_exp_store16(i64, i32)

declare ptr @__asan_memmove(ptr, ptr, i64)

declare ptr @__asan_memcpy(ptr, ptr, i64)

declare ptr @__asan_memset(ptr, i32, i64)

declare void @__asan_handle_no_return()

declare void @__sanitizer_ptr_cmp(i64, i64)

declare void @__sanitizer_ptr_sub(i64, i64)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.shared(ptr) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.private(ptr) #1

declare i64 @__asan_stack_malloc_0(i64)

declare void @__asan_stack_free_0(i64, i64)

declare i64 @__asan_stack_malloc_1(i64)

declare void @__asan_stack_free_1(i64, i64)

declare i64 @__asan_stack_malloc_2(i64)

declare void @__asan_stack_free_2(i64, i64)

declare i64 @__asan_stack_malloc_3(i64)

declare void @__asan_stack_free_3(i64, i64)

declare i64 @__asan_stack_malloc_4(i64)

declare void @__asan_stack_free_4(i64, i64)

declare i64 @__asan_stack_malloc_5(i64)

declare void @__asan_stack_free_5(i64, i64)

declare i64 @__asan_stack_malloc_6(i64)

declare void @__asan_stack_free_6(i64, i64)

declare i64 @__asan_stack_malloc_7(i64)

declare void @__asan_stack_free_7(i64, i64)

declare i64 @__asan_stack_malloc_8(i64)

declare void @__asan_stack_free_8(i64, i64)

declare i64 @__asan_stack_malloc_9(i64)

declare void @__asan_stack_free_9(i64, i64)

declare i64 @__asan_stack_malloc_10(i64)

declare void @__asan_stack_free_10(i64, i64)

declare void @__asan_poison_stack_memory(i64, i64)

declare void @__asan_unpoison_stack_memory(i64, i64)

declare void @__asan_set_shadow_00(i64, i64)

declare void @__asan_set_shadow_01(i64, i64)

declare void @__asan_set_shadow_02(i64, i64)

declare void @__asan_set_shadow_03(i64, i64)

declare void @__asan_set_shadow_04(i64, i64)

declare void @__asan_set_shadow_05(i64, i64)

declare void @__asan_set_shadow_06(i64, i64)

declare void @__asan_set_shadow_07(i64, i64)

declare void @__asan_set_shadow_f1(i64, i64)

declare void @__asan_set_shadow_f2(i64, i64)

declare void @__asan_set_shadow_f3(i64, i64)

declare void @__asan_set_shadow_f5(i64, i64)

declare void @__asan_set_shadow_f8(i64, i64)

declare void @__asan_alloca_poison(i64, i64)

declare void @__asan_allocas_unpoison(i64, i64)

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
define internal void @asan.module_ctor() #2 comdat {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  call void @__asan_register_elf_globals(i64 ptrtoint (ptr @___asan_globals_registered to i64), i64 ptrtoint (ptr @__start_asan_globals to i64), i64 ptrtoint (ptr @__stop_asan_globals to i64))
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_address uwtable }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind uwtable "frame-pointer"="all" }
attributes #3 = { nomerge }

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
!24 = !{!"branch_weights", i32 1, i32 1048575}
!25 = !DILocation(line: 5, column: 35, scope: !26)
!26 = distinct !DILexicalBlock(scope: !10, file: !11, line: 4, column: 5)
!27 = !DILocation(line: 7, column: 6, scope: !10)
!28 = !DILocation(line: 7, column: 13, scope: !10)
!29 = !DILocation(line: 8, column: 1, scope: !10)
